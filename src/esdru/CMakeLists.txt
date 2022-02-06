add_executable(esdru esdru.c)
target_link_libraries(esdru ${M_LIBRARY})


#TEST: ESDRU
add_test(esdru1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/esdru 0.2 test_data/stereo_test.pcm test_data/stereo_test.0.2.test.pcm)
add_test(esdru1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/stereo_test.0.2.test.pcm test_data/stereo_test.0.2.pcm)
