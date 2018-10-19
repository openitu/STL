include_directories(../basop)
include_directories(../eid)
include_directories(../utl)

add_executable(g722demo g722demo.c funcg722.c g722.c ../basop/basop32.c ../basop/control.c ../basop/count.c ../basop/enh1632.c ../eid/softbit.c)
target_link_libraries(g722demo ${M_LIBRARY})

add_executable(encg722 encg722.c funcg722.c g722.c ../basop/basop32.c ../basop/control.c ../basop/count.c ../basop/enh1632.c ../eid/softbit.c)
target_link_libraries(encg722 ${M_LIBRARY})

add_executable(decg722 decg722.c funcg722.c g722.c ../basop/basop32.c ../basop/control.c ../basop/count.c ../basop/enh1632.c ../eid/softbit.c)
target_link_libraries(decg722 ${M_LIBRARY})

add_executable(tstcg722 tstcg722.c funcg722.c funcg722.c ../basop/basop32.c ../basop/control.c ../basop/count.c ../basop/enh1632.c ../eid/softbit.c)
target_link_libraries(tstcg722 ${M_LIBRARY})

add_executable(tstdg722 tstdg722.c funcg722.c funcg722.c ../basop/basop32.c ../basop/control.c ../basop/count.c ../basop/enh1632.c ../eid/softbit.c)
target_link_libraries(tstdg722 ${M_LIBRARY})

add_test(g722demo1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g722demo -q -enc test_data/inpsp.bin test_data/inpsp.bs)
add_test(g722demo1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/inpsp.bs  test_data/codspw.cod 64)

add_test(g722demo2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g722demo -q -dec -mode 1 test_data/codspw.cod test_data/outsp.md1)
add_test(g722demo2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/outsp.md1 test_data/outsp1.bin 64)

add_test(g722demo3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g722demo -q -dec -mode 2 test_data/codspw.cod test_data/outsp.md2)
add_test(g722demo3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/outsp.md2 test_data/outsp2.bin 64)

add_test(g722demo4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g722demo -q -dec -mode 3 test_data/codspw.cod test_data/outsp.md3)
add_test(g722demo4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/outsp.md3 test_data/outsp3.bin 64)

add_test(g722demo5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g722demo -q test_data/inpsp.bin test_data/outsp.e-d)
add_test(g722demo5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/outsp.md1 test_data/outsp.e-d 64)

add_test(tstcg722-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tstcg722 -q test_data/bt1c1.xmt test_data/bt2r1.cod)
add_test(tstcg722-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tstcg722 -q test_data/bt1c2.xmt test_data/bt2r2.cod)
add_test(tstDg722-3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tstdg722 -q test_data/bt2r1.cod test_data/bt3l1.rc1 test_data/bt3h1.rc0)
add_test(tstDg722-4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tstdg722 -q test_data/bt2r1.cod test_data/bt3l1.rc2 test_data/bt3h1.rc0)
add_test(tstDg722-5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tstdg722 -q test_data/bt2r1.cod test_data/bt3l1.rc3 test_data/bt3h1.rc0)
add_test(tstDg722-6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tstdg722 -q test_data/bt2r2.cod test_data/bt3l2.rc1 test_data/bt3h2.rc0)
add_test(tstDg722-7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tstdg722 -q test_data/bt2r2.cod test_data/bt3l2.rc2 test_data/bt3h2.rc0)
add_test(tstDg722-8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tstdg722 -q test_data/bt2r2.cod test_data/bt3l2.rc3 test_data/bt3h2.rc0)
add_test(tstDg722-9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tstdg722 -q test_data/bt1d3.cod test_data/bt3l3.rc1 test_data/bt3h3.rc0)
add_test(tstDg722-10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tstdg722 -q test_data/bt1d3.cod test_data/bt3l3.rc2 test_data/bt3h3.rc0)
add_test(tstDg722-11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tstdg722 -q test_data/bt1d3.cod test_data/bt3l3.rc3 test_data/bt3h3.rc0)

add_test(encg722-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/encg722 -q -byte test_data/inpsp.bin test_data/temp.cod)
add_test(encg722-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/temp.cod test_data/codspw.cod 64)

add_test(decg722-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/decg722 -q -mode 1 -byte test_data/codspw.cod test_data/temp1.out)
add_test(decg722-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/temp1.out test_data/outsp1.bin 64)

add_test(decg722-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/decg722 -q -mode 2 -byte test_data/codspw.cod test_data/temp2.out)
add_test(decg722-2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/temp2.out test_data/outsp2.bin 64)

add_test(decg722-3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/decg722 -q -mode 3 -byte test_data/codspw.cod test_data/temp3.out)
add_test(decg722-3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/temp3.out test_data/outsp3.bin 64)
