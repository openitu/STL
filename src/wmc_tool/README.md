# WMC tool

## Description

The WMC tool allows for automatic complexity and memory instrumentation of speech and audio codecs written in the floating point format. The WMC tool inserts pseudo-instructions for counting the complexity of mathematical operations. The weights of these operations are derived from the macros defined in the [ITU-T STL Manual (Article 18.7, Table 18.3)](https://www.itu.int/rec/T-REC-G.191-202207-I/en).

The WMC tool also inserts instructions for measuring ROM and RAM consumption within the codec. When running an instrumented codec the user receives a report detailing its complexity and maximum ROM and RAM consumption.

The WMC tool can only instrument `.c` files respecting the [C90 standard](https://web.archive.org/web/20200909074736/https://www.pdf-archive.com/2014/10/02/ansi-iso-9899-1990-1/ansi-iso-9899-1990-1.pdf). There are some limitation as to the supported data types, expressions and logical operations that the WMC tool can successfully instrument. Please, refer to the documentation or invoke the WMC tool with the `-i` command-line option:

```
./wmc_tool -i
```

for more details.

## Copyright

This software is protected by copyright law and by international treaties. The source code, and all of its derivations, is provided by VoiceAge Corporation under the "ITU-T Software Tools' General Public License". Please, read the license file or refer to ITU-T Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND AUDIO CODING STANDARDS". Any use of this software is permitted provided that this notice is not removed and that neither the authors nor VoiceAge Corporation are deemed to have made any representations as to the suitability of this software for any purpose nor are held responsible for any defects of this software. THERE IS NO WARRANTY FOR THIS SOFTWARE.

## History

2022-10-15 First release v1.4

## Authors

Guy Richard, VoiceAge Corp.  
Vladimir Malenovsky, VoiceAge Corp. (Vladimir.Malenovsky@USherbrooke.ca)

## Source files
```
 wmc_tool.cpp:         Main program
 c_parser.cpp:         Routines for parsing C functions
 text_utils.cpp:       Text and string processing functions
 output.cpp:           Warning and Error messages
 ```

The files `wmc_auto_h.txt` and `wmc_auto_c.txt` contain functions and definitions in the raw text format. They are processed internally by the WMC tool and converted into `wmc_auto.h` and `wmc_auto.c`, respectively. These files shall not be modified by an external program or converted manually.

## Installation

### Unix-based systems

To build the project on Unix-based platforms invoke the following commands from the top-level directory containing the file `CMakeLists.txt`:
```
mkdir build
cd build
cmake ..
make
```

The binary file `wmc_tool` shall be created in the top-level directory.

### Windows system

To build the project on MS Windows use the `cmake` command with `-G` option specifying the target platform. For example, to build project files for 64-bit MSVC 2019, invoke the following commands from the top-level directory containing the file `CMakeLists.txt`:
```
md build
cd build
cmake -G "Visual Studio 16 2019" -A "x64" ..
msbuild wmc_tool.sln
```

The executable file `wmc_tool.exe` shall be created in the top-level directory. Note, that it is recommended to run these commands from the `Developer Command Prompt for VS2019` opened in `Administrator` mode. This ensures that all paths to libraries including the SDK can be found by the `cmake` command. This can also be verified with the `vswhere` command.

## Testing

To verify the conformance of the WMC tool it's possible to invoke the `ctest` command from the `build` directory. On Windows platforms it may be necessary to append the config type with the `-C` command-line option. If no specific config type has been specified when running the `cmake` command, then `ctest -C Debug` shall be used for testing. This runs a series of pre-defined tests using some exemplary `.c` files located in the `testv/src` folder. The instrumented files are compared with their respective references located in the `testv/ref` folder. In case of test failure it's possible to re-run the test with the `--verbose` command-line option to see the reason of failure. Note, that `ctest` uses the Python wrapper script `testv/test_wmc_tool.py` for copying the source files, running the WMC tool binary (executable) file, comparing the output to the reference and cleaning up the work.

## Usage

The WMC tool may be applied on individual `.c` files or entire directories containing `.c` files. The examples below are provided for Unix-based systems.

To instrument a file named `test.c` run the WMC tool as follows:
```
./wmc_tool test.c
```

To instrument all `.c` file in a directory named `lib_code` run the WMC tool as follows:
```
./wmc_tool "lib_code/*.c"
```
Note, that on Unix-based platforms it's necessary to use `""` quotes when entering file masks on the command-line. 

The program ROM and table (const data) ROM is instrumented automatically in all `.c` files. The WMC tool inserts specific instructions and macros for tracking the maximum stack size and the maximum heap size. To print out the statistics about the total ROM and RAM consumption within the codec the WMC tool shall be invoked with the `-m` command-line option. The statistics are then printed with the pre-defined `print_mem()` function.

The WMC tool generates two files `wmc_auto.h` and `wmc_auto.c` with the `-c` command-line option. These files shall be included in the instrumented codec to successfully interpret the instrumentation macros. The best practice is to include the file `wmc_auto.h` in all `.c` files in the codec. 

