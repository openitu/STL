include_directories(../utl)

add_executable(mnrudemo mnrudemo.c mnru.c ../utl/ugst-utl.c)
target_link_libraries(mnrudemo ${M_LIBRARY})

add_executable(snr calc-snr.c)
target_link_libraries(snr ${M_LIBRARY})

