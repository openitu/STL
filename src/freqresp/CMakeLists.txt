include_directories(../utl)

add_executable(freqresp freqresp.c bmp_utils.c export.c fft.c ../utl/wav_io.c)

target_link_libraries(freqresp ${M_LIBRARY})

add_test(freqresp ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/freqresp -bmp test_data/bmpOut.tst test_data/input.src test_data/input.src test_data/asciiOut.tst)

add_test(freqresp-verify1 ${CMAKE_COMMAND} -E compare_files test_data/bmpOut.ref test_data/bmpOut.tst)
add_test(NAME freqresp-verify2
  COMMAND ${CMAKE_COMMAND}
    -DGOT=${CMAKE_CURRENT_SOURCE_DIR}/test_data/asciiOut.tst
    -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/test_data/asciiOut.ref
    -DLABEL=freqresp_asciiOut
    -P ${CMAKE_CURRENT_SOURCE_DIR}/compare_freqresp_text.cmake
)

