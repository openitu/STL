cmake_minimum_required(VERSION 3.10)
project(wmc_tool)
include(CTest)

if(WIN32)
  # MSVC compiler flags
  add_definitions(
    -D_CRT_SECURE_NO_WARNINGS
    /MP
  )
endif()  

add_executable(wmc_tool c_parser.cpp output.cpp text_utils.cpp wmc_tool.cpp)

# Testing
file(TO_CMAKE_PATH ${CMAKE_SOURCE_DIR}/test_data TEST_DIR)
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



