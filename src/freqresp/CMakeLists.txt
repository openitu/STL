include_directories(../utl)

add_executable(freqresp freqresp.c bmp_utils.c export.c fft.c)

target_link_libraries(freqresp ${M_LIBRARY})

