       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
       CODING STANDARDS". Further copyright notice below.
       =============================================================

This is the readme file for the ITU-T STL module rpeltp. This module has
been based on the implementation done by Jutta Degener and Carsten Bormann,
from the Technische Universitaet Berlin, at the Communications and
Operating Systems Research Group (KBS).

The authors have granted the right to use it provided that the copyright
notices are kept in the files. See file "cpyright" for their copyright
conditions and statements.

This module comprises several files, described below:
```
make-vms.com    Program-making DCL for VAX-C and VMS port of the GNU C compiler
makefile.tcc    Makefile for Borland's tcc
makefile.cl     Makefile for MS Visual C Compiler
makefile.djc    Makefile for the MSDOS port of the GNU-C compiler (DJCPP)
makefile.unx    Unix Makefile, adapted from the original distribution
rpedemo.prj     Borland's BC project file (binary!)

add_test.c      Program for testing the correct working of the functions
                in add.c. Needed only when porting the program to new
                platforms.
add_test.dta    Input test sequence for add_test.c

rpedemo.c       demo program for the rpeltp module
ugstdemo.h      UGST header with user interface macros and other nice stuf.
```

The following are the functions that comprise the rpeltp module:
```
add.c       code.c      config.h    debug.c     decode.c
gsm.h       gsm_crea.c  gsm_deco.c  gsm_dest.c  gsm_enco.c
gsm_expl.c  gsm_impl.c  gsm_opti.c  gsm_prin.c  long_ter.c
lpc.c       preproce.c  private.h   proto.h     rpe.c
rpeltp.c    rpeltp.h    short_te.c  table.c     unproto.h
```

all above but rpeltp.c are from the original distribution (after changes to
ensure portability!). This one has been added to provide "driving" functions
in the UGST style.

File names from the original distribution had to be shortened because of the
DOS limitation for 8 plus 3 letters for file names. See "changes" for a
list of the files with changes (shortened) names.

### Note on Testing
The directory `ts` has a file named `sine.lin`, that is a sinewave. Try make with
the proper makefile and run with the target `sine'. Then you should get no
differences. Please note that this is *not* a thorough validation of the
coder, because this is done using the 5 test sequences sold by ETSI. Because
they are sold, we cannot distribute them here. But the code has been tested,
and passed, the necessary test sequence in at least: Sun [cc/gcc/acc and
SunOs 4.3], VAX/VMS [Vax-C3/gcc], HP-UX [gcc], PC [bcc 16 bit/DJC gcc].
Therefore, use sine.lin to test for rough errors.

NOTE: If you have the test sequences, the makefiles can process them for you,
if they are named seq01 to seq05.

-- <simao@ctd.comsat.com> -------


# Changelog

LOG of the changes introduced by Simao and summary of results.

1. first of all, file names had to fit in DOS limitations. Some names had
   to be truncated, and the mapping is as follows:
```
	gsm_create.c    -> gsm_crea.c
	gsm_decode.c    -> gsm_deco.c
	gsm_destroy.c   -> gsm_dest.c
	gsm_encode.c    -> gsm_enco.c
	gsm_explode.c   -> gsm_expl.c
	gsm_implode.c   -> gsm_impl.c
	gsm_option.c    -> gsm_opti.c
	gsm_print.c     -> gsm_prin.c
	long_term.c     -> long_ter.c
	preprocess.c    -> preproce.c
	short_term.c    -> short_te.c
```

2. gsm.h had added prototypes for all the existing functions

3. add.c had to be changed to make add_test.c work

4. private.h had to be changed to use either true-function or the safe
   pseudo-functions for the GSM_... macros. This was the main reason for
   the errors that were happening.

5. undefs were preceeded by a check of their definition to avoid some
   complaints from VAX-C

6. rpe.c had to be changed in a strange hacked "switch" by its "normal"
   implementation for VMS (compilation error!) -  the code is as it was
   in comments.

7. To keep compatibility, the change in patch 3 to rpe.c
   shall not be
   #define STEP( i, H )    (e[ k + i ] * (long)H)
   but
   #define STEP( i, H )    (e[ k + i ] * (longword)H)

8. long_term.c (now long_ter.c) had to have added some castings, such
   as:
      temp = gsm_norm( (longword)dmax << 16 );
                       ^^^^^^^^^^
   because gsm_norm uses longs and dmax is short: (short)<<16 = 0 always!
   Also, if MSDOS/__MSDOS__ is defined, then USE_TABLE_MUL is
   #undef'd (see tests below).

9. other small changes that I don't recall from my notes. These may include
   type castings, and other changes done in the desperate search for the bugs.


## Simplifications

Since I don't want a compress-like utility, I haven't put my hands in toast.
To ease my work, I've put all src, inc and add-test in one single directory
and simplified the makefile. I also added labels "all", "rpedemo" and "clean",
"anyway".


## Extensions

I added a DCL for compiling on VMS using either gcc or vax-c, depending on
the comments. I have also added a borland bcc make file for the pc, as well
as a gcc makefile for the pc.

Added a demo program and 2 driving routines (currently embedded in the demo
program). With them, interfacing with the test sequence file is
straight-forward.


## Test results

the bcc-compiled version processed correctly all the 5 test vectors for
the following conditions:

(SASR always defined)
```
USE_FLOAT_MUL undefined
USE_FLOAT_MUL defined and FAST undefined
USE_FLOAT_MUL and FAST defined
```

bcc failed compilation when USE_FLOAT_MUL undefined and USE_TABLE_MUL defined
because the table matrix is greater than 64kbytes! (No way out!)

When compiled by bcc with both USE_FLOAT_MUL and FAST undefined, bcc
failed passing the test sequences.

Besides compilation worked for USE_FLOAT_MUL and FAST defined, in fact, all
the runs with the test sequences were made in the "compatible mode", i.e.,
with the state variable structure fuield "fast" always set as 0. Therefore,
it was not tested. Although, it is known from the original authors that
the fast mode does not work in Unix (32bit), hence it is very likely not
to work with DOS either!

NeedFunctionPrototypes was always defined for bcc, gcc, acc, and vax-c,
and undefined for unix cc.

Since gcc is the same implementation ported across platforms, it should
work as for SunOS, but it was not tested.

The program worked properly for the following environments (see summary below):
SunOS 4.3:	cc, acc, gcc
MS/DOS: 	bcc, gcc (djcpp)
VAX/VMS: 	cc (Vax-c), gcc

Summary of the tests:
```
-------------------------------------------------------------------------
Environment  Compiler   Symbols defined (Note: SASR always def'd)
                        USE_FLOAT_MUL	FAST    USE_TABLE_MUL  Worked?
-------------------------------------------------------------------------
SunOS 4.3:  cc              yes          no         -           yes
                            yes         yes         -           yes*
                            no            -         no          yes
                            no            -         yes         yes
            acc same as for cc in SunOS 4.3                     yes
            gcc same as for cc in SunOS 4.3                     yes

MS/DOS:     bcc             yes          no         -           yes
                            yes         yes         -           yes
                            no            -         no          no
                            no            -         yes         no
            gcc             Not tested other than yes/no/-      yes

VAX/VMS:    Vax-c           same as for cc in SunOS 4.3         yes
            gcc             Not tested other than yes/no/-      yes

-------------------------------------------------------------------------
* Note: all the runs were made in the compatible mode, therefore the
        fast computation of the data was not performed!
-------------------------------------------------------------------------
```

Therefore, the safest approach is to compile always with `USE_FLOAT_MUL`
defined and with and FAST defined if appropriate for speed!

In Vax-C there were warnings relating to function names longer than 31
characters. No way to circumvent this, but it is not a problem because the
31 first letters generate unique names anyway. Because of this, link will
also complain!

The test sequence files had the byte order swapped in the Sun workstation
because of the Big Endian data orientation.

-- <simao@cpqd.ansp.br> -- 8/Apr/94
