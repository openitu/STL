add_executable(spdemo spdemo.c ugst-utl.c)
target_link_libraries(spdemo ${M_LIBRARY})

add_executable(scaldemo scaldemo.c ugst-utl.c)
target_link_libraries(scaldemo ${M_LIBRARY})

add_executable(signal-diff signal-diff.c)

#TODO Input file is not _yet_ there.
add_test(scaldemo1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scaldemo -q -trunc ../is54/test_data/voice.src test_data/voice.tru 256 1 0 0.5941352)
add_test(scaldemo1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../sv56/test_data/voice.ltl test_data/voice.tru)

add_test(scaldemo2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scaldemo -q -premask -round ../is54/test_data/voice.src test_data/voice.rou 256 1 0 0.5941352)
add_test(scaldemo2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -equiv 1 ../sv56/test_data/voice.ltl test_data/voice.rou)

add_test(scaldemo3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scaldemo -q -round ../is54/test_data/voice.src test_data/voice.rnp 256 1 0 0.5941352)
add_test(scaldemo3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -equiv 1 test_data/voice.rnp test_data/voice.rou)

add_test(spdemo1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 16 -right ps test_data/spref.src test_data/sptst-r.s16 100)
add_test(spdemo1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/spref-r.s16 test_data/sptst-r.s16)

add_test(spdemo2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 15 -right ps test_data/spref.src test_data/sptst-r.s15 100)
add_test(spdemo2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/spref-r.s15 test_data/sptst-r.s15)

add_test(spdemo3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 12 -right ps test_data/spref.src test_data/sptst-r.s12 100)
add_test(spdemo3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/spref-r.s12 test_data/sptst-r.s12)

add_test(spdemo4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 16 -left  ps test_data/spref.src test_data/sptst-l.s16 100)
add_test(spdemo4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/spref-l.s16 test_data/sptst-l.s16)

add_test(spdemo5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 15 -left  ps test_data/spref.src test_data/sptst-l.s15 100)
add_test(spdemo5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/spref-l.s15 test_data/sptst-l.s15)

add_test(spdemo6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 12 -left  ps test_data/spref.src test_data/sptst-l.s12 100)
add_test(spdemo6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/spref-l.s12 test_data/sptst-l.s12)

add_test(spdemo7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 16 -nosync ps test_data/spref.src test_data/sptst-s.s16 100)
add_test(spdemo7-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/spref-s.s16 test_data/sptst-s.s16)

add_test(spdemo8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 15 -right sp  test_data/sptst-r.s15 test_data/sptst-r.p15 100)
add_test(spdemo8-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/spref-r.p15 test_data/sptst-r.p15)
add_test(spdemo8-2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sptst-r.p15 test_data/spref.src 100)

add_test(spdemo10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 12 -right sp  test_data/sptst-r.s12 test_data/sptst-r.p12 100)
add_test(spdemo10-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sptst-r.p12 test_data/spref.src 100)
add_test(spdemo10-2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/spref-r.p12 test_data/sptst-r.p12)

add_test(spdemo11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 16 -nosync sp test_data/sptst-s.s16 test_data/sptst-s.p16 100)
add_test(spdemo11-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sptst-s.p16 test_data/spref.src)

add_test(spdemo12 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 16 -right sp  test_data/sptst-r.s16 test_data/sptst-r.p16 100)
add_test(spdemo13 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 16 -left  sp  test_data/sptst-l.s16 test_data/sptst-l.p16 100)
add_test(spdemo13-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sptst-l.p16 test_data/sptst-r.p16)
add_test(spdemo13-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sptst-l.p16 test_data/sptst-s.p16)

add_test(spdemo14 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 12 -left  sp  test_data/sptst-l.s12 test_data/sptst-l.p12 100)
add_test(spdemo14-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sptst-l.p12 test_data/spref.src 100)
add_test(spdemo14-2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/spref-l.p12 test_data/sptst-l.p12)

add_test(spdemo15 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/spdemo -q -r 15 -left  sp  test_data/sptst-l.s15 test_data/sptst-l.p15 100)
add_test(spdemo15-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sptst-l.p15 test_data/spref.src 100)
add_test(spdemo15-2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/spref-l.p15 test_data/sptst-l.p15)
