cmake_minimum_required(VERSION 3.3)
project(wmc_tool)
include(CTest)

if(UNIX)
  # To build and test in Unix:
  # mkdir build
  # cd build
  # cmake ..
  # make
  # ctest --verbose
  
  # GCC compiler options
  add_compile_options(-Wall)
  
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})  # set directory for binary/executable file
endif()

if(WIN32)

  # To build and test in Windows (MSVC 2019:
  # md build
  # cd build
  # cmake -G "Visual Studio 16 2019" -A "x64" ..
  # msbuild wmc_tool.sln
  # ctest --verbose
  
  # MSVC compiler flags
  add_definitions(
    -D_CRT_SECURE_NO_WARNINGS
    /MP
  )
  
if (MSVC_IDE)
  # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
  # set_target_properties (${NAME} PROPERTIES PREFIX "../")
endif()
  
  # CMake sets /W3 by default, until CMake version 3.15. Instead of setting /W4 separately, replace it in existing settings
  string(REGEX REPLACE "/W3" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  set(CMAKE_SUPPRESS_REGENERATION true)     # don't generate ZERO_CHECK
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)  # put ALL_BUILD in a sub-folder in the MSVC navigator
  set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "wmc_tool" )  # set the default startup project
  set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})   # set directory for .pdb file
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}>)  # set directory for binary/executable file
endif()



file(GLOB srcs
    ${CMAKE_SOURCE_DIR}/source/*.cpp
    ${CMAKE_SOURCE_DIR}/source/*.h
)
add_executable(wmc_tool ${srcs})
target_include_directories(wmc_tool PRIVATE ${CMAKE_SOURCE_DIR}/source)


if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()

# Testing
file(TO_CMAKE_PATH ${CMAKE_SOURCE_DIR}/testv TEST_DIR)
file(TO_CMAKE_PATH ${TEST_DIR}/test_wmc_tool.py TEST_WMC_TOOL_SCRIPT)

add_test(NAME Dry_Run COMMAND wmc_tool -h)
add_test(NAME Dry_Run2 COMMAND wmc_tool -i)
add_test(NAME Test_Single_File COMMAND python ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -s src -r ref -o out -v test_file1.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME Test_Single_File2 COMMAND python ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> test_file2.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME Test_Multiple_Files COMMAND python ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v test_file*.c test_g728durb.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME Test_Basop_File COMMAND python ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v test_basop.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME Test_Basop_File2 COMMAND python ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v test_basop32.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME Test_ROM_File COMMAND python ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v -m main1.c test_rom.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME Test_ROM_File2 COMMAND python ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v -m main2.c test*.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME Test_Desinstrument_File COMMAND python ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v -d di_test_file1.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME Test_Desinstrument_ROM_File COMMAND python ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v -d di_test_rom.c WORKING_DIRECTORY ${TEST_DIR})



