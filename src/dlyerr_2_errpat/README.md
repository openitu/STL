       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
       CODING STANDARDS".
       =============================================================

# dlyerr_2_errpat

Delay-and-error profile to frame-erasure pattern conversion tool. Reads a
delay/error profile (one entry per packet, giving network delay in ms and a
loss flag) and emits an error pattern suitable for use with the STL
`eid-xor` tool. Supports both fixed-delay and bounded-loss-rate jitter
buffer management emulation modes.

Provided by Fraunhofer IIS via 3GPP Tdoc S4-121077 (TSGS4#70, Chicago,
13–17 Aug 2012), in support of the EVS reference codec processing plan.

## Source files

    dlyerr_2_errpat.c: Tool source and main(); single-file program.

## Usage

    dlyerr_2_errpat [options]

      -i <inputfile>         delay/error profile (required)
      -o <outputfile>        error pattern (required)
      -L <length>            output length in frames
      -s <offset>            shift/offset in frames into the profile
      -f <frames_per_packet> 1 or 2
      -l <late_loss_rate>    bounded-loss-rate mode, percent
      -d <jbm_delay_ms>      constant-JBM-delay mode, milliseconds
      -b                     byte-oriented G.192 format (0x21 ok, 0x20 lost)
      -w                     word-oriented G.192 (0x6b21 ok, 0x6b20 lost)
      -c                     append LF in text format (one entry per line;
                             was the default in V1.0)

Exactly one of `-l` or `-d` must be supplied.

## Examples (from S4-121077)

Apply MTSI delay/error profile 1, 2, or 3 (fixed JBM delay) to a G.192
bitstream:

    dlyerr_2_errpat -d 200 -f 1 -w -s YYY -i dly_err_profile_XXX.dat -o epXXX.g192
    eid-xor -fer g192bsin epXXX.g192 g192bsout

Profiles 4 or 6 (bounded loss rate, 1 frame/packet):

    dlyerr_2_errpat -l 1 -f 1 -w -s YYY -i dly_err_profile_XXX.dat -o epXXX.g192

Profile 5 (bounded loss rate, 2 frames/packet):

    dlyerr_2_errpat -l 1 -f 2 -w -s YYY -i dly_err_profile_5.dat -o ep5.g192

`YYY` is a random offset into the profile (see the STL `random` tool for
generating reproducible offsets).
