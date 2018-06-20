
import matplotlib as mpl
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cbook as cbook
import pylab as pylab
import os
import shutil
#import pandas as pd

basedir='../test_data/'

def read_datafile(file_name):
    # the skiprows keyword is for heading, but I don't know if trailing lines
    # can be specified
    data = np.loadtxt(basedir + 'basop_ip_op_plot.csv', delimiter=',', skiprows=2)
    return data

def plot_graphs(s, offset, i):

    if ((i>13 and i<20) or i==28 or i==29 or (i>51 and i<58)  or i==67):

        data = read_datafile(basedir + 'basop_ip_op_plot.csv')
    
        x = data[1001+offset:2000+offset,0]
        
        plt.rc('ytick',labelsize=10)
        plt.rc('xtick',labelsize=10)
 
        fig = plt.figure()
        ax1 = fig.add_subplot(411)
        
        ax1.set_title("input")    
        #text(.5,.9,'input', horizontalalignment='center',transform=ax3.transAxes)
 
        ax1.plot(x, c='r', label='input')
        
        #leg = ax1.legend()
        
        x = data[1001+offset:2000+offset,3]
        y = data[1001+offset:2000+offset,4]
        ax2 = fig.add_subplot(412)
        ax2.plot(x, c='b', label='dut output')
        ax2.set_title("dut output")    
        #text(.5,.9,'DUT output', horizontalalignment='center',transform=ax3.transAxes)
        
        ax3 = fig.add_subplot(413)
        ax3.plot(y, c='g', label='ref output')
        ax3.set_title("ref output")    
        #text(.5,.9,'REF output', horizontalalignment='center',transform=ax3.transAxes)
        
        x = data[1001+offset:2000+offset,5]
        ax4 = fig.add_subplot(414)
        ax4.plot(x, c='r', label='error')
        ax4.set_title("error")    
        #text(.5,.9,'Error', horizontalalignment='center',transform=ax3.transAxes)
        
        ax1.grid(); ax2.grid(); ax3.grid(); ax4.grid()
        fig.tight_layout() 
        pylab.savefig(basedir + './plots/pat1'+basops[i]+'.png')
        #plt.show()
        
        
        #plot for pattern 2
        fig = plt.figure()
        ax1 = fig.add_subplot(411)
        
        x = data[1+offset:1000+offset,0]
        ax1.set_title("input")
        ax1.plot(x, c='r', label='input')
        
        x = data[1+offset:1000+offset,3]
        ax2 = fig.add_subplot(412)
        ax2.plot(x, c='b', label='dut output')
        ax2.set_title("dut output")
        
        y = data[1+offset:1000+offset,4]
        ax3 = fig.add_subplot(413)
        ax3.plot(y, c='g', label='ref output')
        ax3.set_title("ref output")
        
        x = data[1+offset:1000+offset,5]
        ax4 = fig.add_subplot(414)
        ax4.plot(x, c='r', label='error')
        ax4.set_title("error")
        
        ax1.grid(); ax2.grid(); ax3.grid(); ax4.grid()
        fig.tight_layout()     
        pylab.savefig(basedir + './plots/pat2'+basops[i]+'.png')
        #plt.show()
        
        
        
        #plot for pattern 3
        fig = plt.figure()
        ax1 = fig.add_subplot(411)
        
        x = data[2001+offset:3000+offset,0]
        ax1.set_title("input")
        ax1.plot(x, c='r', label='input')
        
        x = data[2001+offset:3000+offset,3]
        ax2 = fig.add_subplot(412)
        ax2.plot(x, c='b', label='dut output')
        ax2.set_title("dut output")
        
        y = data[2001+offset:3000+offset,4]
        ax3 = fig.add_subplot(413)
        ax3.plot(y, c='g', label='ref output')
        ax3.set_title("ref output")
        
        x = data[2001+offset:3000+offset,5]
        ax4 = fig.add_subplot(414)
        ax4.plot(x, c='r', label='error')
        ax4.set_title("error")
        
        ax1.grid(); ax2.grid(); ax3.grid(); ax4.grid()
        fig.tight_layout()      
        pylab.savefig(basedir + './plots/pat3'+basops[i]+'.png')
        #plt.show()
        
        
        
        #plot for pattern 4
        fig = plt.figure()
        ax1 = fig.add_subplot(411)
        
        x = data[3001+offset:4000+offset,0]
        ax1.set_title("input")
        ax1.plot(x, c='r', label='input')
        
        x = data[3001+offset:4000+offset,3]
        ax2 = fig.add_subplot(412)
        ax2.plot(x, c='b', label='dut output')
        ax2.set_title("dut output")
        
        y = data[3001+offset:4000+offset,4]
        ax3 = fig.add_subplot(413)
        ax3.plot(y, c='g', label='ref output')
        ax3.set_title("ref output")
        
        x = data[3001+offset:4000+offset,5]
        ax4 = fig.add_subplot(414)
        ax4.plot(x, c='r', label='error')
        ax4.set_title("error")
        
        ax1.grid(); ax2.grid(); ax3.grid(); ax4.grid()
        fig.tight_layout()     
        pylab.savefig(basedir + './plots/pat4'+basops[i]+'.png')
        #plt.show()
        plt.close('all')
        
    elif (i ==0 or (i>2 and i<14) or i==20 or (i>24 and i<28) or (i>33 and i<38) or (i>41 and i<52) or (i>59 and i<66)):
        data = read_datafile(basedir + 'basop_ip_op_plot.csv')
      
        plt.rc('ytick',labelsize=9)
        plt.rc('xtick',labelsize=9)
        
        fig = plt.figure()
        ax1 = fig.add_subplot(511)        
        x = data[1001+offset:2000+offset,0]
        ax1.set_title("input1")
        ax1.plot(x, c='r', label='input1')
        
        ax5 = fig.add_subplot(512)
        x = data[1001+offset:2000+offset,1]
        ax5.set_title("input2")
        ax5.plot(x, c='g', label='input2')
        
        x = data[1001+offset:2000+offset,3]  
        ax2 = fig.add_subplot(513)
        ax2.plot(x, c='b', label='dut output')
        ax2.set_title("dut output")    
        
        y = data[1001+offset:2000+offset,4]
        ax3 = fig.add_subplot(514)
        ax3.plot(y, c='g', label='ref output')
        ax3.set_title("ref output")    
        
        x = data[1001+offset:2000+offset,5]
        ax4 = fig.add_subplot(515)
        ax4.plot(x, c='r', label='error')
        ax4.set_title("error")    
        #text(.5,.9,'Error', horizontalalignment='center',transform=ax3.transAxes)
        
        ax1.grid(); ax2.grid(); ax3.grid(); ax4.grid(); ax5.grid()
        fig.tight_layout() 
        pylab.savefig(basedir + './plots/pat1'+basops[i]+'.png')
        #plt.show()
        
        
        #plot for pattern 2
        fig = plt.figure()
        ax1 = fig.add_subplot(511)        
        x = data[1+offset:1000+offset,0]
        ax1.set_title("input1")
        ax1.plot(x, c='r', label='input1')
        
        ax5 = fig.add_subplot(512)        
        x = data[1+offset:1000+offset,1]
        ax5.set_title("input2")
        ax5.plot(x, c='g', label='input2')        
        
        x = data[1+offset:1000+offset,3]
        ax2 = fig.add_subplot(513)
        ax2.plot(x, c='b', label='dut output')
        ax2.set_title("dut output")
        
        y = data[1+offset:1000+offset,4]
        ax3 = fig.add_subplot(514)
        ax3.plot(y, c='g', label='ref output')
        ax3.set_title("ref output")
        
        x = data[1+offset:1000+offset,5]
        ax4 = fig.add_subplot(515)
        ax4.plot(x, c='r', label='error')
        ax4.set_title("error")
        
        ax1.grid(); ax2.grid(); ax3.grid(); ax4.grid(); ax5.grid()
        fig.tight_layout()     
        pylab.savefig(basedir + './plots/pat2'+basops[i]+'.png')
        #plt.show()
        
        
        
        #plot for pattern 3
        fig = plt.figure()
        ax1 = fig.add_subplot(511)        
        x = data[2001+offset:3000+offset,0]
        ax1.set_title("input1")
        ax1.plot(x, c='r', label='input1')
        
        ax5 = fig.add_subplot(512)        
        x = data[2001+offset:3000+offset,1]
        ax5.set_title("input2")
        ax5.plot(x, c='g', label='input2')        
        
        x = data[2001+offset:3000+offset,3]
        ax2 = fig.add_subplot(513)
        ax2.plot(x, c='b', label='dut output')
        ax2.set_title("dut output")
        
        y = data[2001+offset:3000+offset,4]
        ax3 = fig.add_subplot(514)
        ax3.plot(y, c='g', label='ref output')
        ax3.set_title("ref output")
        
        x = data[2001+offset:3000+offset,5]
        ax4 = fig.add_subplot(515)
        ax4.plot(x, c='r', label='error')
        ax4.set_title("error")
        
        ax1.grid(); ax2.grid(); ax3.grid(); ax4.grid(); ax5.grid()
        fig.tight_layout()     
        pylab.savefig(basedir + './plots/pat3'+basops[i]+'.png')
        #plt.show()
        
        
        
        #plot for pattern 4
        fig = plt.figure()
        ax1 = fig.add_subplot(511)        
        x = data[3001+offset:4000+offset,0]
        ax1.set_title("input1")
        ax1.plot(x, c='r', label='input1')
        
        ax5 = fig.add_subplot(512)        
        x = data[3001+offset:4000+offset,1]
        ax5.set_title("input2")
        ax5.plot(x, c='g', label='input2')        
        
        x = data[3001+offset:4000+offset,3]
        ax2 = fig.add_subplot(513)
        ax2.plot(x, c='b', label='dut output')
        ax2.set_title("dut output")
        
        y = data[3001+offset:4000+offset,4]
        ax3 = fig.add_subplot(514)
        ax3.plot(y, c='g', label='ref output')
        ax3.set_title("ref output")
        
        x = data[3001+offset:4000+offset,5]
        ax4 = fig.add_subplot(515)
        ax4.plot(x, c='r', label='error')
        ax4.set_title("error")
        
        ax1.grid(); ax2.grid(); ax3.grid(); ax4.grid(); ax5.grid()
        fig.tight_layout()     
        pylab.savefig(basedir + './plots/pat4'+basops[i]+'.png')
        #plt.show()
        plt.close('all')   

    elif (i==1 or i==2 or (i>20 and i<25) or (i>29 and i<34) or (i>37 and i<42) or i==58 or i==59 or i==66):
        data = read_datafile(basedir + 'basop_ip_op_plot.csv')
      
        plt.rc('ytick',labelsize=10)
        plt.rc('xtick',labelsize=10)
        
        fig = plt.figure()
        ax1 = fig.add_subplot(321)        
        x = data[1001+offset:2000+offset,0]
        ax1.set_title("input1")
        ax1.plot(x, c='r', label='input1')
        
        ax5 = fig.add_subplot(323)
        x = data[1001+offset:2000+offset,1]
        ax5.set_title("input2")
        ax5.plot(x, c='g', label='input2')
        
        ax6 = fig.add_subplot(325)
        x = data[1001+offset:2000+offset,2]
        ax6.set_title("input3")
        ax6.plot(x, c='b', label='input6')
        
        x = data[1001+offset:2000+offset,3]  
        ax2 = fig.add_subplot(322)
        ax2.plot(x, c='b', label='dut output')
        ax2.set_title("dut output")    
        
        y = data[1001+offset:2000+offset,4]
        ax3 = fig.add_subplot(324)
        ax3.plot(y, c='g', label='ref output')
        ax3.set_title("ref output")    
        
        x = data[1001+offset:2000+offset,5]
        ax4 = fig.add_subplot(326)
        ax4.plot(x, c='r', label='error')
        ax4.set_title("error")    
        #text(.5,.9,'Error', horizontalalignment='center',transform=ax3.transAxes)
        
        ax1.grid(); ax2.grid(); ax3.grid(); ax4.grid(); ax5.grid(); ax6.grid()
        fig.tight_layout() 
        pylab.savefig(basedir + './plots/pat1'+basops[i]+'.png')
        #plt.show()
        
        
        #plot for pattern 2
        fig = plt.figure()
        ax1 = fig.add_subplot(321)        
        x = data[1+offset:1000+offset,0]
        ax1.set_title("input1")
        ax1.plot(x, c='r', label='input1')
        
        ax5 = fig.add_subplot(323)        
        x = data[1+offset:1000+offset,1]
        ax5.set_title("input2")
        ax5.plot(x, c='g', label='input2')

        ax6 = fig.add_subplot(325)        
        x = data[1+offset:1000+offset,2]
        ax6.set_title("input3")
        ax6.plot(x, c='b', label='input3')        
        
        x = data[1+offset:1000+offset,3]
        ax2 = fig.add_subplot(322)
        ax2.plot(x, c='b', label='dut output')
        ax2.set_title("dut output")
        
        y = data[1+offset:1000+offset,4]
        ax3 = fig.add_subplot(324)
        ax3.plot(y, c='g', label='ref output')
        ax3.set_title("ref output")
        
        x = data[1+offset:1000+offset,5]
        ax4 = fig.add_subplot(326)
        ax4.plot(x, c='r', label='error')
        ax4.set_title("error")
        
        ax1.grid(); ax2.grid(); ax3.grid(); ax4.grid(); ax5.grid(); ax6.grid()
        fig.tight_layout()     
        pylab.savefig(basedir + './plots/pat2'+basops[i]+'.png')
        #plt.show()
        
        
        
        #plot for pattern 3
        fig = plt.figure()
        ax1 = fig.add_subplot(321)        
        x = data[2001+offset:3000+offset,0]
        ax1.set_title("input1")
        ax1.plot(x, c='r', label='input1')
        
        ax5 = fig.add_subplot(323)        
        x = data[2001+offset:3000+offset,1]
        ax5.set_title("input2")
        ax5.plot(x, c='g', label='input2')        

        ax6 = fig.add_subplot(325)        
        x = data[2001+offset:3000+offset,2]
        ax6.set_title("input3")
        ax6.plot(x, c='b', label='input3')            
        
        x = data[2001+offset:3000+offset,3]
        ax2 = fig.add_subplot(322)
        ax2.plot(x, c='b', label='dut output')
        ax2.set_title("dut output")
        
        y = data[2001+offset:3000+offset,4]
        ax3 = fig.add_subplot(324)
        ax3.plot(y, c='g', label='ref output')
        ax3.set_title("ref output")
        
        x = data[2001+offset:3000+offset,5]
        ax4 = fig.add_subplot(326)
        ax4.plot(x, c='r', label='error')
        ax4.set_title("error")
        
        ax1.grid(); ax2.grid(); ax3.grid(); ax4.grid(); ax5.grid(); ax6.grid()
        fig.tight_layout()     
        pylab.savefig(basedir + './plots/pat3'+basops[i]+'.png')
        #plt.show()
        
        
        
        #plot for pattern 4
        fig = plt.figure()
        ax1 = fig.add_subplot(321)        
        x = data[3001+offset:4000+offset,0]
        ax1.set_title("input1")
        ax1.plot(x, c='r', label='input1')
        
        ax5 = fig.add_subplot(323)        
        x = data[3001+offset:4000+offset,1]
        ax5.set_title("input2")
        ax5.plot(x, c='g', label='input2')        
        
        ax6 = fig.add_subplot(325)        
        x = data[3001+offset:4000+offset,1]
        ax6.set_title("input3")
        ax6.plot(x, c='b', label='input3')            
        
        x = data[3001+offset:4000+offset,3]
        ax2 = fig.add_subplot(322)
        ax2.plot(x, c='b', label='dut output')
        ax2.set_title("dut output")
        
        y = data[3001+offset:4000+offset,4]
        ax3 = fig.add_subplot(324)
        ax3.plot(y, c='g', label='ref output')
        ax3.set_title("ref output")
        
        x = data[3001+offset:4000+offset,5]
        ax4 = fig.add_subplot(326)
        ax4.plot(x, c='r', label='error')
        ax4.set_title("error")
        
        ax1.grid(); ax2.grid(); ax3.grid(); ax4.grid(); ax5.grid(); ax6.grid()
        fig.tight_layout()     
        pylab.savefig(basedir + './plots/pat4'+basops[i]+'.png')
        #plt.show()
        plt.close('all')

basops = ["W_mult_16_16", "W_mac_16_16", "W_msu_16_16", "W_add", "W_sub", "W_add_nosat", "W_sub_nosat", "W_shl", "W_shr", "W_shl_nosat", "W_shr_nosat", "W_lshl", "W_lshr", "W_shl_sat_l", "W_sat_l", "W_sat_m", "W_round48_L", "W_round64_L", "W_round32_s", "W_norm", "W_mult0_16_16", "W_mac0_16_16", "W_msu0_16_16", "W_mac_32_16", "W_msu_32_16", "W_mult_32_16", "W_mult_32_32", "W_mult0_32_32", "W_neg", "W_abs",  "Madd_32_16", "Madd_32_32", "Madd_32_16_r", "Madd_32_32_r", "Mpy_32_16_1", "Mpy_32_32", "Mpy_32_32_r", "Mpy_32_16_r", "Msub_32_16", "Msub_32_16_r", "Msub_32_32", "Msub_32_32_r", "CL_add", "CL_sub", "CL_msu_j", "CL_mac_j", "CL_multr_32x32", "CL_multr_32x16", "C_add", "C_sub", "C_multr", "C_scale", "CL_negate", "CL_mul_j", "C_negate", "C_mul_j", "C_mac_r", "C_msu_r", "CL_shr", "CL_shl", "C_shr", "C_shl", "CL_scale_32", "CL_scale", "CL_dscale", "CL_dscale_32", "CL_round32_16"]


if not os.path.exists(basedir + './plots/'):
    os.makedirs(basedir + './plots/')

for i in range(0, 67):
#for i in range(18, 19):
    plot_graphs(basops[i], 4000*i, i)
 
