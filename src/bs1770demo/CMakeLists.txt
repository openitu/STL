add_executable(bs1770demo bs1770demo.c)
target_link_libraries(bs1770demo ${M_LIBRARY})


#TEST: BS1770DEMO
add_test(bs1770demo1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bs1770demo -nchan 6 test_data/sine997_0dBFS.pcm test_data/sine997_0dBFS.26LKFS.test.pcm)
add_test(bs1770demo1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -equiv 27 -q test_data/sine997_0dBFS.26LKFS.test.pcm test_data/sine997_0dBFS.26LKFS.pcm)

add_test(bs1770demo2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bs1770demo -lev -16 -nchan 6 -conf 000L11 test_data/sine_noise_test.pcm test_data/sine_noise_test.16LKFS.000L11.test.pcm)
add_test(bs1770demo2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -equiv 145 -q test_data/sine_noise_test.16LKFS.000L11.test.pcm test_data/sine_noise_test.16LKFS.000L11.pcm)

add_test(bs1770demo3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bs1770demo -lev -16 -conf 11L000 test_data/sine_noise_test.pcm test_data/sine_noise_test.16LKFS.11L000.test.pcm)
add_test(bs1770demo3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -equiv 159 -q test_data/sine_noise_test.16LKFS.11L000.test.pcm test_data/sine_noise_test.16LKFS.11L000.pcm)

add_test(bs1770demo4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bs1770demo -lev -26 test_data/sine_ramp.pcm test_data/sine_ramp.26LKFS.test.pcm)
add_test(bs1770demo4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -equiv 115 -q test_data/sine_ramp.26LKFS.test.pcm test_data/sine_ramp.26LKFS.pcm)

add_test(bs1770demo5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bs1770demo -rms -lev -26 test_data/sine_ramp.pcm test_data/sine_ramp.26LKFSrms.test.pcm)
add_test(bs1770demo5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -equiv 115 -q test_data/sine_ramp.26LKFSrms.test.pcm test_data/sine_ramp.26LKFSrms.pcm)
