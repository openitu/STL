       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
       CODING STANDARDS".
       =============================================================

This text describes the necessary files for the UGST MNRU module as described in ITU-T Rec. P.810.

# Source files

`mnru.h`: Prototypes and definitions for NB, WB and P50 FB MNRU routines and data structures.
			
`mnru.c`: Functions for MNRU operation; this is the module itself. 


# Demo and support files

`mnrudemo.c`:   This is ONLY a demonstration program for the NB and WB MNRU module. Depends on `ugstdemo.h`, `mnru.h` and `mnru.c`.

`p50fbmnru.c`:   This is a demonstration program for P.50 Fullband MNRU module. Depends on `ugstdemo.h`, `mnru.h` and `mnru.c`.

`ugstdemo.h`:   Prototypes and definitions for UGST demo programs (in `../utl`).

`calc-snr.c`:   SNR calculation function

`snr.c`:        Driving program for SNR calculation

`ugst-utl.c`:   Contains conversion routines (found in directory utl)

# P.50 Fullband MNRU demo - `bin/p50fbmnru`

```
P.50 Fullband MNRU - STL2023

  P.50 Fullband MNRU shapes the gaussian noise with an average speech power spectrum as ITU-T Rec. P.50.
  Requires 48kHz sampling rate.

  Usage: p50fbmnru <inputfile> <outputfile> <Q/dB> <Mode> [dcFilter] [--overflow]

      Mode M:   Modulated Noise
           N:   Noise only
           S:   Signal only

      Options:
         dcFilter  0 to disable DC removal Filter (default)
                   1 for 115 Hz -3 dB cutoff (legacy filter, same as P.50 FB MNNU prior 2023
                   2 for 60 Hz -3 dB cutoff
                   3 for 30 Hz -3 dB cutoff
                   4 for 15 Hz -3 dB cutoff
 
         --overflow  int16 overflow (legacy, same as P.50 FB MNNU prior 2023)
                     if undefined, int16 are clamped (default)

```

# Narrowband and wideband MNRU - `bin/mnrudemo`

```
Compiling options: 
	- Using new random number generator
	- DC-removal filter enabled
	- Low-pass filter in the output

MNRU.C - Version 2.2 of 02.Feb.2010 
Demonstration program for generating files with modulated
noise added based on UGST's MNRU module, which is based in the
Recommendation P.81 (Blue Book).

Usage:
$ MNRUDEMO (no parameters) --> to display this help message,
or
$ MNRUDEMO [-options] filin filout blk 1stblk blkno desiredQ [mode]

where:
 filin      input filename [sample format: 16 bit, 2-complement]
 filout     output filename [sample format: 16 bit, 2-complement]
 blk        block-size, in samples [default: 256 samples/block]
 1stblk     number of first block to process [default: 1st]
 blkno      total of blocks to process [default: all]
 desiredQ   desired signal-to-modulated-noise ratio in dB [def=100]
 mode       MNRU operation mode: `S'ignal-only, `N'oise-only,
            or `M'odulated-noise [default: modulated-noise]
Options:
 -q         quiet operation; don't print progress info
 -Q xdB     define Q value as xdB [default: 100dB]
 -noise     define MNRU mode as noise-only
 -signal    define MNRU mode as signal-only
 -mod       define MNRU mode as modulated noise (default)
```
