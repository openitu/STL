include_directories(../utl)

add_executable(mnrudemo mnrudemo.c mnru.c ../utl/ugst-utl.c)
target_link_libraries(mnrudemo ${M_LIBRARY})

add_executable(snr calc-snr.c)
target_link_libraries(snr ${M_LIBRARY})

#TEST: MNRU
add_test(mnrudemo1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mnrudemo -q test_data/sine.src test_data/sine.q00 256 1 20 00)
add_test(mnrudemo1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sine-q00.unx test_data/sine.q00)

add_test(mnrudemo2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mnrudemo -q test_data/sine.src test_data/sine.q05 256 1 20 05)
add_test(mnrudemo2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sine-q05.unx test_data/sine.q05)

add_test(mnrudemo3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mnrudemo -q test_data/sine.src test_data/sine.q10 256 1 20 10)
add_test(mnrudemo3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sine-q10.unx test_data/sine.q10)

add_test(mnrudemo4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mnrudemo -q test_data/sine.src test_data/sine.q15 256 1 20 15)
add_test(mnrudemo4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sine-q15.unx test_data/sine.q15)

add_test(mnrudemo5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mnrudemo -q test_data/sine.src test_data/sine.q20 256 1 20 20)
add_test(mnrudemo5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sine-q20.unx test_data/sine.q20)

add_test(mnrudemo6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mnrudemo -q test_data/sine.src test_data/sine.q25 256 1 20 25)
add_test(mnrudemo6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sine-q25.unx test_data/sine.q25)

add_test(mnrudemo7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mnrudemo -q test_data/sine.src test_data/sine.q30 256 1 20 30)
add_test(mnrudemo7-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sine-q30.unx test_data/sine.q30)

add_test(mnrudemo8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mnrudemo -q test_data/sine.src test_data/sine.q35 256 1 20 35)
add_test(mnrudemo8-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sine-q35.unx test_data/sine.q35)

add_test(mnrudemo9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mnrudemo -q test_data/sine.src test_data/sine.q40 256 1 20 40)
add_test(mnrudemo9-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sine-q40.unx test_data/sine.q40)

add_test(mnrudemo10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mnrudemo -q test_data/sine.src test_data/sine.q45 256 1 20 45)
add_test(mnrudemo10-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sine-q45.unx test_data/sine.q45)

add_test(mnrudemo11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mnrudemo -q test_data/sine.src test_data/sine.q50 256 1 20 50)
add_test(mnrudemo11-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sine-q50.unx test_data/sine.q50)

add_test(mnrudemo12 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mnrudemo -q test_data/sine.src test_data/sine.q99 256 1 20 150)
add_test(mnrudemo12-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sine-q99.unx test_data/sine.q99)

#TEST: Compute SNR for MNRU files (no automatic verification data available)
add_test(snr1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snr -q test_data/sine.q99 test_data/sine.q00)
add_test(snr2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snr -q test_data/sine.q99 test_data/sine.q05)
add_test(snr3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snr -q test_data/sine.q99 test_data/sine.q10)
add_test(snr4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snr -q test_data/sine.q99 test_data/sine.q15)
add_test(snr5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snr -q test_data/sine.q99 test_data/sine.q20)
add_test(snr6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snr -q test_data/sine.q99 test_data/sine.q25)
add_test(snr7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snr -q test_data/sine.q99 test_data/sine.q30)
add_test(snr8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snr -q test_data/sine.q99 test_data/sine.q35)
add_test(snr9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snr -q test_data/sine.q99 test_data/sine.q40)
add_test(snr10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snr -q test_data/sine.q99 test_data/sine.q45)
add_test(snr11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snr -q test_data/sine.q99 test_data/sine.q50)

