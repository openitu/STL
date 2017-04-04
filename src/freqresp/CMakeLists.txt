include_directories(../utl)

add_executable(freqresp freqresp.c bmp_utils.c export.c fft.c)

target_link_libraries(freqresp ${M_LIBRARY})

add_test(freqresp ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/freqresp -bmp test_data/bmpOut.tst test_data/input.src test_data/input.src test_data/asciiOut.tst)

add_test(freqresp-verify1 ${CMAKE_COMMAND} -E compare_files test_data/bmpOut.ref test_data/bmpOut.tst)
add_test(freqresp-verify2 ${CMAKE_COMMAND} -E compare_files test_data/asciiOut.ref test_data/asciiOut.tst)

