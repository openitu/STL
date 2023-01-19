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
