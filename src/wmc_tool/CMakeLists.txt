cmake_minimum_required(VERSION 3.5)
project(wmc_tool)
include(CTest)

if(WIN32)
  # MSVC compiler flags
  add_definitions(
    -D_CRT_SECURE_NO_WARNINGS
    /MP
  )
endif()  

add_executable(wmc_tool parsing_defs.h c_parser.h c_parser.cpp output.h output.cpp text_utils.h text_utils.cpp constants.h wmc_tool.h wmc_tool.cpp)
find_package (Python COMPONENTS Interpreter)

# Testing
file(TO_CMAKE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test_data TEST_DIR)
file(TO_CMAKE_PATH ${TEST_DIR}/test_wmc_tool.py TEST_WMC_TOOL_SCRIPT)

add_test(NAME wmc_tool_dry_run COMMAND wmc_tool -h)
add_test(NAME wmc_tool_dry_run2 COMMAND wmc_tool -i)
add_test(NAME wmc_tool_test_single_file COMMAND ${Python_EXECUTABLE} ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -s src -r ref -o out -v test_file1.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME wmc_tool_test_single_file2 COMMAND ${Python_EXECUTABLE} ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> test_file2.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME wmc_tool_test_multiple_files COMMAND ${Python_EXECUTABLE} ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v test_file*.c test_g728durb.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME wmc_tool_test_basop_file COMMAND ${Python_EXECUTABLE} ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v test_basop.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME wmc_tool_test_basop_file2 COMMAND ${Python_EXECUTABLE} ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v test_basop32.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME wmc_tool_test_rom_file COMMAND ${Python_EXECUTABLE} ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v -m main1.c test_rom.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME wmc_tool_test_rom_file2 COMMAND ${Python_EXECUTABLE} ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v -m main2.c test*.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME wmc_tool_test_desinstrument_file COMMAND ${Python_EXECUTABLE} ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v -d di_test_file1.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME wmc_tool_test_desinstrument_rom_file COMMAND ${Python_EXECUTABLE} ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -v -d di_test_rom.c WORKING_DIRECTORY ${TEST_DIR})
add_test(NAME wmc_tool_test_wmc_auto_files COMMAND ${Python_EXECUTABLE} ${TEST_WMC_TOOL_SCRIPT} -e $<TARGET_FILE:wmc_tool> -c ${TEST_DIR} test_file3.c WORKING_DIRECTORY ${TEST_DIR})



