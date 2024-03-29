/*===================================================================*
*  Complexity evaluation tool for floating point C Code
*  Software Release 1.0 (2009-09)
*
* (C) 2009 VoiceAge Corporation. All rights reserved.
*
* VoiceAge Corp. makes no representation nor warranty in regard to
* the accuracy, completeness or sufficiency of The Software, nor
* shall VoiceAge Corp. be held liable for any damages whatsoever
* relating to use of said Software.
*===================================================================*/

This Complexity evaluation tool for floating point C Code enables to
estimate number of WMOPS and Program ROM of a floating point implementation
of speech and audio codecs. Note that for the correct estimation you need to
set properly the factor FLC_FRAMELEN in "flc.h" file. Also an estimation of
the complexity that would be obtained after conversion of a floating point
source code into the corresponding fixed point implementation is computed.

To use this tool in existing code, include the files "flc.c" and "flc.h" into
your project. The last file "flc_example.c" serves as an example and guideline
for the use of the tool in existing code. Note that once the code is
instrumented using the complexity counter macros and functions, the
compilation switch DONT_COUNT can be activated to suppress the functionality
of the tool with no need of removing the complexity counter macros and
functions.

The subdirectory "workspace" contains two makefiles that were prepared and
tested for compilation of the example "flc_example.c" under Cygwin/gcc and
Windows/MSVC. Below you can find the output screen when executing the example.

/// beginning of the example output ///

===== Call Graph and total ops per function =====

Function                     Calls              Ops         Ops/Call
-----------
ROOT                             1                0                0
-Autocorr                      100           975900             9759
--Set_Zero                     200             4100             20.5
-Lev_dur                       100            99600              996

===== Program Memory Usage by Function =====

Function            ADD  MULT   MAC  MOVE STORE LOGIC SHIFT BRNCH   DIV
-----------
Set_Zero              0     0     0     1     0     0     0     0     0
Autocorr              1     4    17     0    21     0     0     2     0
Lev_dur               0     2     5     5     3     0     1     1     2
ROOT                  0     0     0     0     0     0     0     0     0

-----------
totals                1     6    22     6    24     0     1     3     2

--------------------------------

Function           SQRT TRANC  FUNC  LOOP   IND   PTR  MISC
-----------
Set_Zero              0     0     0     1     0     1     0
Autocorr              0     0     2     2     1     6     0
Lev_dur               0     0     0     3     4     5     0
ROOT                  0     0     2     0     0     0     0

-----------
totals                0     0     4     6     5    12     0


===== SUMMARY =====

Total Ops: 1.0796e+06
Total Program ROM usage: 83  (word)

===== Per Frame Summary =====
Number of Frames: 100
Average Ops/frame: 10796.00   Max Ops/frame: 10796


===== ESTIMATED COMPLEXITY (Frame length is 20.00 ms) =====
Maximum complexity: 0.539800 WMOPS
Average complexity: 0.539800 WMOPS

Estimated fixed point complexity with 1.1 scaling factor:
Maximum complexity: 0.593780 WMOPS
Average complexity: 0.593780 WMOPS

/// end of the example output ///
