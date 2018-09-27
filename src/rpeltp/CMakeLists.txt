include_directories(../g711)
include_directories(../utl)

add_executable(rpedemo rpedemo.c add.c code.c debug.c decode.c long_ter.c lpc.c preproce.c rpe.c gsm_dest.c gsm_deco.c gsm_enco.c gsm_expl.c gsm_impl.c gsm_crea.c gsm_prin.c gsm_opti.c rpeltp.c short_te.c table.c ../g711/g711.c)
target_link_libraries(rpedemo ${M_LIBRARY})

add_executable(test-add add_test.c)
target_link_libraries(test-add ${M_LIBRARY})

add_test(test-add ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test-add test_data/add_test.dta)

#Generate test files using g711demo
add_test(rpelt-g711demo1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo a lilo test_data/sine.lin test_data/sine.a 160 1 20)
add_test(rpelt-g711demo2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo a loli test_data/sine.a test_data/sine.a-a 160 1 20)
add_test(rpelt-g711demo3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo u lilo test_data/sine.lin test_data/sine.u 160 1 20)
add_test(rpelt-g711demo4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo u loli test_data/sine.u test_data/sine.u-u 160 1 20)

#Test for u-law
add_test(rpeltdemo-ulaw1-encode ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpedemo -enc test_data/sine.u-u test_data/sineu-u.rpe)
add_test(rpeltdemo-ulaw1-decode ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpedemo -dec test_data/sineu-u.rpe test_data/sineu-u.l)
add_test(rpeltdemo-ulaw2-encode ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpedemo -enc -u test_data/sine.u test_data/sineu.rpe)
add_test(rpeltdemo-ulaw-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sineu-u.rpe test_data/sineu.rpe 160)
add_test(rpeltdemo-ulaw2-decode ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpedemo -dec -u test_data/sineu-u.rpe test_data/sineu-u.u)
add_test(rpeltdemo-ulaw2-g711demo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo u lilo test_data/sineu-u.l test_data/sineu-ul.u 160 1 20)
add_test(rpeltdemo-ulaw2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sineu-u.u test_data/sineu-ul.u 160)

add_test(rpeltdemo-ulaw3-e_d-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpedemo test_data/sine.u-u test_data/sineu-e_d-u.l)
add_test(rpeltdemo-ulaw3-e_d-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpedemo -u test_data/sine.u test_data/sineu-e_d-u.u)
add_test(rpeltdemo-ulaw3-e_d-g711demo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo u lilo test_data/sineu-e_d-u.l test_data/sineu-e_d-ul.u 160 1 20)
add_test(rpeltdemo-ulaw3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sineu-e_d-u.u test_data/sineu-e_d-ul.u 160)

#Test for A-Law
add_test(rpeltdemo-alaw1-encode ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpedemo -enc test_data/sine.a-a test_data/sinea-a.rpe)
add_test(rpeltdemo-alaw1-decode ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpedemo -dec test_data/sinea-a.rpe test_data/sinea-a.l)
add_test(rpeltdemo-alaw2-encode ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpedemo -enc -a test_data/sine.a test_data/sinea.rpe)
add_test(rpeltdemo-alaw-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sinea-a.rpe test_data/sinea.rpe 160)
add_test(rpeltdemo-alaw2-decode ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpedemo -dec -a test_data/sinea-a.rpe test_data/sinea-a.a)
add_test(rpeltdemo-alaw2-g711demo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo a lilo test_data/sinea-a.l test_data/sinea-al.a 160 1 20)
add_test(rpeltdemo-alaw2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sinea-a.a test_data/sinea-al.a 160)

add_test(rpeltdemo-alaw3-e_d-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpedemo test_data/sine.a-a test_data/sinea-e_d-a.l)
add_test(rpeltdemo-alaw3-e_d-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpedemo -a test_data/sine.a test_data/sinea-e_d-a.a)
add_test(rpeltdemo-alaw3-e_d-g711demo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo a lilo test_data/sinea-e_d-a.l test_data/sinea-e_d-al.a 160 1 20)
add_test(rpeltdemo-alaw3-e_d-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sinea-e_d-a.a test_data/sinea-e_d-al.a 160)

