README for the _ITU-T Software Tool Library_ (STL)
===

The STL provides software for speech- and audio-related speech processing, including narrowband (telephony), wideband ans super-wideband applications. 
This includes codecs, noise generators, filter, etc.

The STL is standardized by ITU-T.
Please see ITU-T Recommendation G.191 "Software Tools for Speech and Audio Coding Standards".

Build procedure
---
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

Licenses
---
The STL, and all of its derivations, is subject to the "ITU-T General Public License", which is a modified version of the GPL2.
Please read the file `LICENSE.md` for the terms and conditions.

The code in `is54` and `rpeltp` have additional copyright issues.
Please read the appropriate files in these directories.
