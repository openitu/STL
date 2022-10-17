#!/usr/bin/python

from __future__ import print_function

import os
import argparse
import sys
import subprocess
import glob
import shutil
# import tempfile
import re

# when running the script in IPython use %run -G to prevent automatic expansion of wildcards


# Main routine
if __name__ == '__main__':

    # print("here")
    # sys.exit(0)
    
    parser = argparse.ArgumentParser(description='Wrapper script for running wmc_tool', usage='use "%(prog)s --help" for more information', formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('file_list', metavar='file1|dir1 file2|dir2 ...', type=str, nargs='+',
                        help='space-separated list of filenames or directories in the source directory\nwildcards may be used, e.g. ar*.c dec/*.c\nnote: *.c is assumed as the default mask for directories')
    parser.add_argument('-v', '--verbose', action='store_true', help='print warnings and other information messages')                        
    parser.add_argument('-e', '--wmc_tool_binary', metavar='wmc_tool_binary', type=str, dest='wmc_tool_binary', default='..', help='complete path to the wmc_tool binary file [executable]')
    parser.add_argument('-s', '--srcdir', metavar='srcdir', type=str, dest='srcdir', default='./src', help='source directory relative to the current working directory')
    parser.add_argument('-r', '--refdir', metavar='refdir', type=str, dest='refdir', default='./ref', help='reference file directory relative to the current working directory')
    parser.add_argument('-o', '--outdir', metavar='outdir', type=str, dest='outdir', default='./out', help='output file directory relative to the current working directory')
    parser.add_argument('-i', '--info-only', action='store_true', help='only print instrumentation information')                        
    parser.add_argument('-d', '--desinstrument', action='store_true', help='desintrument only')                        
    parser.add_argument('-m', '--rom', metavar='rom_file', dest='rom_file', type=str, help='instruments code for Program and Const (Table) ROM calculation\nnote: filename shall point to an existing .c file containing the print_mem() function')
    parser.add_argument('-b', '--no-backup', action='store_true', help='no backup of original files')                        
    parser.add_argument('-c', '--wmc_auto_dir', metavar='wmc_auto_dir', type=str, dest='wmc_auto_dir', help='copy wmc_auto.[h|c] to a user-specified directory')
    args = parser.parse_args()

    # create temporary directory
    # tmp_dir = tempfile.mkdtemp()
    # tmp_dir = './tmp'
    # if os.path.exists(tmp_dir):
        # shutil.rmtree(tmp_dir)
    # os.makedirs(tmp_dir)
    

    # normalize all paths to platform-specific format
    if args.wmc_tool_binary:
        args.wmc_tool_binary = os.path.normpath(args.wmc_tool_binary)
    if args.refdir:
        args.refdir = os.path.normpath(args.refdir)
    if args.srcdir:
        args.srcdir = os.path.normpath(args.srcdir)
    if args.outdir:
        args.outdir = os.path.normpath(args.outdir)

    # change directory for wmc_auto_dir
    if args.wmc_auto_dir:
        args.wmc_auto_dir = args.outdir

    # print("Current directory: %s" % os.path.normpath(os.getcwd()))
    # print("Directory where binary file is: %s" % args.bindir)
    # sys.exit(0)
    
    # create output directory, if not existing (otherwise clean it up)
    if not os.path.exists(args.outdir):
        os.makedirs(args.outdir)
    else:
        for filename in os.listdir(args.outdir):
            file_path = os.path.join(args.outdir, filename)
            try:
                if os.path.isfile(file_path) or os.path.islink(file_path):
                    os.unlink(file_path)
                elif os.path.isdir(file_path):
                    shutil.rmtree(file_path)
            except Exception as e:
                print('Failed to delete %s. Reason: %s' % (file_path, e))
        
    # copy files from source directory to output directory
    for i, item in enumerate(args.file_list):
        for file in glob.glob(os.path.join(args.srcdir, item)):
            shutil.copy(file, args.outdir)
        args.file_list[i] = os.path.join(args.outdir, os.path.basename(item))

    # print(os.path.normpath(args.rom_file))
    # sys.exit(0)
    
    # copy also Const_Data_PROM_file
    if args.rom_file:
        args.rom_file = os.path.normpath(args.rom_file)
        shutil.copy(os.path.join(args.srcdir, args.rom_file), args.outdir)
        args.rom_file = os.path.join(args.outdir, os.path.basename(args.rom_file))
                
    # build the command-line for the WMC tool
    cmdline = [args.wmc_tool_binary]
    if args.verbose:
        cmdline.append("-v")
    if args.info_only:
        cmdline.append("-i")
    if args.desinstrument:
        cmdline.append("-d")
    if args.rom_file:
        cmdline.append("-m")
        cmdline.append(args.rom_file)
    if args.no_backup:
        cmdline.append("-b")
    if args.wmc_auto_dir:
        cmdline.append("-c")
        cmdline.append(args.wmc_auto_dir)
    for file in args.file_list:
        cmdline.append(file)
    
    # print(' '.join(cmdline))
    # sys.exit(0)
    
    # run the WMC tool
    popen = subprocess.Popen(cmdline, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    popen.wait()
    output = popen.stdout.read()
    print(output.decode())
    # output = popen.stderr.read()
    # print(output.decode())
       
    # compare all output files against their references
    results = []
    if popen.returncode == 0:
        # prepare list of files to compare
        file_list_cmp = args.file_list
        if args.rom_file:
            file_list_cmp += [args.rom_file]
        if args.wmc_auto_dir:
            file_list_cmp += [os.path.join(args.outdir, "wmc_auto.h")]
            file_list_cmp += [os.path.join(args.outdir, "wmc_auto.c")]
            
        # compare all output files against reference files
        for item in (args.file_list):
            item = os.path.abspath(item)
            for out_file in glob.glob(item):
                ref_file = out_file.replace(args.outdir, args.refdir)
                
                # read both files
                with open(ref_file, 'r') as f:
                    ref_file_contents = f.read()
                with open(out_file, 'r') as f:
                    out_file_contents = f.read()
                    
                # strip off spaces
                ref_file_contents = re.sub(r"[\n\t\s]*", "", ref_file_contents)
                out_file_contents = re.sub(r"[\n\t\s]*", "", out_file_contents)

                # print(ref_file_contents)
                # print("\n\n")
                # print(out_file_contents)
                # sys.exit(-1)
                
                # compare the two files
                print('Comparing {} with {}: '.format(ref_file, out_file), end='')
                if (ref_file_contents == out_file_contents):
                    results += ["identical"]
                    print("identical")
                else:
                    results += ["different"]
                    print("different")

    # delete output directory
    if os.path.exists(args.outdir):
        shutil.rmtree(args.outdir)
                
    # exit, if WMC tool ran with error or if at least one file was different during comparison
    if popen.returncode != 0 or "different" in results:
        sys.exit(1)
   
  