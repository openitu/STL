#!/bin/bash

# 1) Download test files from http://www.itu.int/dms_pub/itu-r/oth/11/02/R11020000010030ZIPM.zip 
#    and put in the folder R11020000010001ZIPM
# 2) Download test files from https://extranet.itu.int/rsg-meetings/sg6/wp6c/Share/Forms/Display%20View.aspx?RootFolder=%2Frsg%2Dmeetings%2Fsg6%2Fwp6c%2FShare%2F2016%2D2019%2F2016%2D10%20meeting%2FSWG%206C%2D3%20Audio%20Matters%20October%202016%2FDG3%2D1%20Loudness%2FAudio%20Files
#    and put in the folder R11020000010001ZIPM 
#
# Requires - CopyAudio http://www-mmsp.ece.mcgill.ca/Documents/Downloads/AFsp/
#          - bc

INDIR=R11020000010001ZIPM

N=47

files=(
1770-2_Comp_18LKFS_FrequencySweep.wav
1770-2_Comp_23LKFS_10000Hz_2ch.wav
1770-2_Comp_23LKFS_1000Hz_2ch.wav
1770-2_Comp_23LKFS_100Hz_2ch.wav
1770-2_Comp_23LKFS_2000Hz_2ch.wav
1770-2_Comp_23LKFS_25Hz_2ch.wav
1770-2_Comp_23LKFS_500Hz_2ch.wav
1770-2_Comp_23LKFS_ChannelCheckCentre.wav
1770-2_Comp_23LKFS_ChannelCheckLFE.wav
1770-2_Comp_23LKFS_ChannelCheckLeft.wav
1770-2_Comp_23LKFS_ChannelCheckLs.wav
1770-2_Comp_23LKFS_ChannelCheckRight.wav
1770-2_Comp_23LKFS_ChannelCheckRs.wav
1770-2_Comp_23LKFS_SummingTest.wav
1770-2_Comp_24LKFS_10000Hz_2ch.wav
1770-2_Comp_24LKFS_1000Hz_2ch.wav
1770-2_Comp_24LKFS_100Hz_2ch.wav
1770-2_Comp_24LKFS_2000Hz_2ch.wav
1770-2_Comp_24LKFS_25Hz_2ch.wav
1770-2_Comp_24LKFS_500Hz_2ch.wav
1770-2_Comp_24LKFS_ChannelCheckCentre.wav
1770-2_Comp_24LKFS_ChannelCheckLFE.wav
1770-2_Comp_24LKFS_ChannelCheckLeft.wav
1770-2_Comp_24LKFS_ChannelCheckLs.wav
1770-2_Comp_24LKFS_ChannelCheckRight.wav
1770-2_Comp_24LKFS_ChannelCheckRs.wav
1770-2_Comp_24LKFS_SummingTest.wav
1770-2_Comp_AbsGateTest.wav
1770-2_Comp_RelGateTest.wav
1770-2_Conf_6ch_VinCntr-23LKFS.wav
1770-2_Conf_6ch_VinCntr-24LKFS.wav
1770-2_Conf_6ch_VinL+R-23LKFS.wav
1770-2_Conf_6ch_VinL+R-24LKFS.wav
1770-2_Conf_6ch_VinL-R-C-23LKFS.wav
1770-2_Conf_6ch_VinL-R-C-24LKFS.wav
1770-2_Conf_Mono_Voice+Music-23LKFS.wav
1770-2_Conf_Mono_Voice+Music-24LKFS.wav
1770-2_Conf_Stereo_VinL+R-23LKFS.wav
1770-2_Conf_Stereo_VinL+R-24LKFS.wav
1770Conf-23LKFS-10channel.wav
1770Conf-23LKFS-12channel.wav
1770Conf-23LKFS-24channel.wav
1770Conf-23LKFS-8channel.wav
1770Conf-24LKFS-10channel.wav
1770Conf-24LKFS-12channel.wav
1770Conf-24LKFS-24channel.wav
1770Conf-24LKFS-8channel.wav
)

conf=(
0
00
00
00
00
00
00
000L11
000L11
000L11
000L11
000L11
000L11
000L11
00
00
00
00
00
00
000L11
000L11
000L11
000L11
000L11
000L11
000L11
00
00
000L11
000L11
000L11
000L11
000L11
000L11
0
0
00
00
000L110000
000L11000000
000L11000L11000000110000
000L1100
000L110000
000L11000000
000L11000L11000000110000
000L1100
)

target_level=(
-18
-23
-23
-23
-23
-23
-23
-23
-23
-23
-23
-23
-23
-23
-24
-24
-24
-24
-24
-24
-24
-24
-24
-24
-24
-24
-24
-69
-10
-23
-24
-23
-24
-23
-24
-23
-24
-23
-24
-23
-23
-23
-23
-24
-24
-24
-24
)

for i in {0..46};do
    rm -f tmp.raw
    copyaudio -F noheader $INDIR/${files[i]} tmp.raw > /dev/null 2>&1
    lkfs=`bs1770demo.exe -conf ${conf[i]} tmp.raw | grep "Input level" | cut -f 2 -d':'`
    diff=`echo ${target_level[i]} $lkfs | gawk '{print $1-$2}'`
    neg=`echo "$diff<0" | bc -l`
    if [ $neg -eq 1 ]; then
        diff=`echo "0 - $diff" | bc -l`
    fi
    comparison_res=`echo "$diff > 0.1" | bc -l`
    if [ $comparison_res -eq 1 ]; then
        res=FAIL
    else
        res=PASS
    fi
    echo $res: ${files[i]}
done


