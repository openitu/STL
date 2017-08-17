README for the _ITU-T Software Tool Library_ (STL)
===

The STL provides software for speech- and audio-related speech processing, including narrowband (telephony), wideband ans super-wideband applications. 
This includes codecs, noise generators, filter, etc.

The STL is standardized by ITU-T in [Recommendation ITU-T G.191 "Software tools for speech and audio coding standardization"](http://itu.int/ITU-T/G.191).

A few more lines on the STL mission statement and target audience, as well as the proposed development and change process can be found below. 

# Build procedure
The STL should compile on state-of-the-art Microsoft Windows, MacOS, and Unix-based systems.
More recent additions to the STL have not been tested with less recent platforms.

To build the STL tools, you need:
* [CMake](https://cmake.org/), and
* a modern C-Compiler (such as [GCC](https://gcc.gnu.org/), [Clang](https://clang.llvm.org/), or [MSVC](http://landinghub.visualstudio.com/visual-cpp-build-tools)).

Not so recent C compilers might also work (not tested).

```shell
#Download source code
git clone https://github.com/openitu/STL

#Enter directory
cd STL

#Generate platform dependent build scripts using CMake
#For using a specific generator see CMake parameter "-G"
cmake .

##Build STL tools
cmake --build .

##Run tests (optional)
ctest
```

# Licenses
The STL, and all of its derivations, is subject to the "ITU-T General Public License", which is a modified version of the GPL2.
Please read the file `LICENSE.md` for the terms and conditions.

The code in `is54` and `rpeltp` have additional copyright issues.
Please read the appropriate files in these directories.

# Mission statement
The vision of the ITU-T software tools library (STL) was to provide a set of common, coherent and portable signal processing tools to facilitate the development of speech and audio coding algorithms, in particular within the standardization environment in ITU.

The refocusing of the STL as an open source project continues to aim at providing a library of portable, interworkable, modular, reliable and well-documented software routines, now led and maintained by an open, wide community of experts developing and testing speech and audio coding algorithms, to and that satisfies its evolving needs. 

Future work may extend the STL to include image and video coding algorithms.

# Target audience
The primary audience using, maintaining and extending the STL is primarily constituted of standards makers and the scientific community developing and testing speech and audio coding algorithms. This includes students of electrical engineering and computer sciences.

# Development and change process
We distinguish between (1) the STL code development in the open source environment, and (2) the maintenance and update of Recommendation ITU-T G.191 in the standardization environment.

## Open source environment (GitHub):
*   It follows the best practices for open source code development
*   Participation is open and encouraged to anyone interested in the development of the STL, independent of ITU membership.
*   The open source environment will have three branches of the STL:
    1.   Master branch — the latest ITU-T approved version of the STL (starting with STL2009);
    2.   Development branch — to advance the STL code and add new features; and
    3.   Submission branch — to submit new stable code to SG12.
*   Proposals submitted to the development branch document motivation and intended merits.
*   Any tools in the library should satisfy the principles outlined in clauses 2.3 and 2.4 of the G.191: ITU-T Software Tool Library 2009 User's manual, in particular with respect to testing of portability and compliance.
*   New tools: 
    *   should not overlap in functionality provided by other tools already in the library, unless there is community consensus on the benefits of such approach;
    *   should provide evidence of and tools for portability;
    *   should provide a sufficient number of software tests;
    *   should provide a license with freedoms at least as those found in the ITU-T STL copyleft license (see [Recommendation ITU-T G.191 (03/2010)](https://www.itu.int/rec/T-REC-G.191-201003-I), Annex B). Compatible, less restrictive licenses, could be used.
*   Changes to existing tools: 
    *   should provide sufficient evidence that the changes do not break existing code (in particular, backward compatibility), unless there is community consensus that this is the best approach;
    *   should respect the license of the tool.
    *   The development branch is used for discussion and consensus building on a stable version, for inclusion in the submission branch.
    
## Standardization environment ([ITU-T Study Group 12](https://itu.int/go/tsg12)):
*   Participation is limited to those who can participate in the standardization work in ITU-T SG12.
*   The Editor of ITU-T G.191 monitors the change process in the open source environment. 
*   Upon availability of sufficient stable material in the submission branch, the Editor of ITU-T G.191 submits a TD to Q2/12 summarizing the proposed changes and related discussion, and provides a snapshot of the submission branch as basis for a revised version of ITU-T G.191.
*   The TD is considered by Q2/12, and action is taken following the alternative approval procedure (AAP) for ITU-T Recommendations.
*   The Editor of ITU-T G.191 provides feedback on the actions taken to the developers in the open source environment, and, upon approval of a revised version of G.191, updates the master branch of the open source environment.
