include_directories(../utl)

add_executable(reverb reverb.c reverb-lib.c)
target_link_libraries(reverb ${M_LIBRARY})

#NOTE: Test depends on endianess!
add_test(reverb-little ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/reverb test_data/input.src test_data/irtest_le.IR test_data/output.tst)
#add_test(reverb-big ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/reverb test_data/ input.src test_data/irtest_be.IR test_data/output.tst)

add_test(reverb-verify1 ${CMAKE_COMMAND} -E compare_files test_data/output.ref test_data/output.tst)

