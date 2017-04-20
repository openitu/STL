include_directories(../basop)
include_directories(../eid)
include_directories(../g711)
include_directories(../utl)

add_executable(discard discard.c)
target_link_libraries(discard ${M_LIBRARY})

add_executable(g727demo g727demo.c g727.c ../g711/g711.c)
target_link_libraries(g727demo ${M_LIBRARY})

add_test(g727-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -enc -law A test_data/speech.a-s   test_data/speech44.iad)
add_test(g727-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/speech44.iad test_data/speech44.ais)

add_test(g727-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -dec -law A test_data/speech44.iad test_data/speech44.out)
add_test(g727-2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/speech44.out test_data/speech44.acs)

add_test(g727-3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -e_d -law A test_data/speech.a-s   test_data/speech44.ouk)
add_test(g727-3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/speech44.out test_data/speech44.ouk)

#Verification tests: 40k
add_test(g727-40k-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -enc -law u test_data/ovr.m test_data/rv54_m.iad)
add_test(g727-40k-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv54_m.i test_data/rv54_m.iad)

add_test(g727-40k-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -enc -law u test_data/nrm.m test_data/rn54_m.iad)
add_test(g727-40k-2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn54_m.i test_data/rn54_m.iad)

add_test(g727-40k-3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -enc -law A test_data/ovr.a test_data/rv54_a.iad)
add_test(g727-40k-3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv54_a.i test_data/rv54_a.iad)

add_test(g727-40k-4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -enc -law A test_data/nrm.a test_data/rn54_a.iad)
add_test(g727-40k-4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn54_a.i test_data/rn54_a.iad)

add_test(g727-40k-5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -dec -law u test_data/rv54_m.i test_data/rv54_m.out)
add_test(g727-40k-5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv54_m.o test_data/rv54_m.out)

add_test(g727-40k-6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -dec -law a test_data/rv54_a.i test_data/rv54_a.out)
add_test(g727-40k-6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv54_a.o test_data/rv54_a.out)

add_test(g727-40k-7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -dec -law u test_data/rn54_m.i test_data/rn54_m.out)
add_test(g727-40k-7-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn54_m.o test_data/rn54_m.out)

add_test(g727-40k-8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -dec -law a test_data/rn54_a.i test_data/rn54_a.out)
add_test(g727-40k-8-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn54_a.o test_data/rn54_a.out)

add_test(g727-40k-9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -dec -law u test_data/i40 test_data/ri54_m.out)
add_test(g727-40k-9-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri54_m.o test_data/ri54_m.out)

add_test(g727-40k-9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -dec -law a test_data/i40 test_data/ri54_a.out)
add_test(g727-40k-9-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri54_a.o test_data/ri54_a.out)

add_test(g727-40k-10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -dec -law A test_data/rv54_m.i test_data/rv54_c.out)
add_test(g727-40k-10-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv54_c.o test_data/rv54_c.out)

add_test(g727-40k-11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -dec -law u test_data/rv54_a.i test_data/rv54_x.out)
add_test(g727-40k-11-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv54_x.o test_data/rv54_x.out)

add_test(g727-40k-12 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -dec -law A test_data/rn54_m.i test_data/rn54_c.out)
add_test(g727-40k-12-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn54_c.o test_data/rn54_c.out)

add_test(g727-40k-13 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -dec -law u test_data/rn54_a.i test_data/rn54_x.out)
add_test(g727-40k-13-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn54_x.o test_data/rn54_x.out)

add_test(g727-40k-14 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -enc -law u test_data/ovr.m test_data/rv53_m.iad)
add_test(g727-40k-14-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv53_m.i test_data/rv53_m.iad)

add_test(g727-40k-15 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -enc -law u test_data/nrm.m test_data/rn53_m.iad)
add_test(g727-40k-15-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn53_m.i test_data/rn53_m.iad)

add_test(g727-40k-16 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -enc -law A test_data/ovr.a test_data/rv53_a.iad)
add_test(g727-40k-16-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv53_a.i test_data/rv53_a.iad)

add_test(g727-40k-17 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -enc -law A test_data/nrm.a test_data/rn53_a.iad)
add_test(g727-40k-17-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn53_a.i test_data/rn53_a.iad)

add_test(g727-40k-18 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -dec -law u test_data/rv53_m.i test_data/rv53_m.out)
add_test(g727-40k-18-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv53_m.o test_data/rv53_m.out)

add_test(g727-40k-19 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -dec -law a test_data/rv53_a.i test_data/rv53_a.out)
add_test(g727-40k-19-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv53_a.o test_data/rv53_a.out)

add_test(g727-40k-20 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -dec -law u test_data/rn53_m.i test_data/rn53_m.out)
add_test(g727-40k-20-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn53_m.o test_data/rn53_m.out)

add_test(g727-40k-21 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -dec -law a test_data/rn53_a.i test_data/rn53_a.out)
add_test(g727-40k-21-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn53_a.o test_data/rn53_a.out)

add_test(g727-40k-22 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -dec -law u test_data/i40 test_data/ri53_m.out)
add_test(g727-40k-22-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri53_m.o test_data/ri53_m.out)

add_test(g727-40k-23 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -dec -law a test_data/i40 test_data/ri53_a.out)
add_test(g727-40k-23-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri53_a.o test_data/ri53_a.out)

add_test(g727-40k-24 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -dec -law A test_data/rv53_m.i test_data/rv53_c.out)
add_test(g727-40k-24-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv53_c.o test_data/rv53_c.out)

add_test(g727-40k-25 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -dec -law u test_data/rv53_a.i test_data/rv53_x.out)
add_test(g727-40k-25-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv53_x.o test_data/rv53_x.out)

add_test(g727-40k-26 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -dec -law A test_data/rn53_m.i test_data/rn53_c.out)
add_test(g727-40k-26-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn53_c.o test_data/rn53_c.out)

add_test(g727-40k-27 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -dec -law u test_data/rn53_a.i test_data/rn53_x.out)
add_test(g727-40k-27-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn53_x.o test_data/rn53_x.out)

add_test(g727-40k-28 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -enc -law u test_data/ovr.m test_data/rv52_m.iad)
add_test(g727-40k-28-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv52_m.i test_data/rv52_m.iad)

add_test(g727-40k-29 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -enc -law u test_data/nrm.m test_data/rn52_m.iad)
add_test(g727-40k-29-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn52_m.i test_data/rn52_m.iad)

add_test(g727-40k-30 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -enc -law A test_data/ovr.a test_data/rv52_a.iad)
add_test(g727-40k-30-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv52_a.i test_data/rv52_a.iad)

add_test(g727-40k-31 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -enc -law A test_data/nrm.a test_data/rn52_a.iad)
add_test(g727-40k-31-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn52_a.i test_data/rn52_a.iad)

add_test(g727-40k-32 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -dec -law u test_data/rv52_m.i test_data/rv52_m.out)
add_test(g727-40k-32-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv52_m.o test_data/rv52_m.out)

add_test(g727-40k-33 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -dec -law a test_data/rv52_a.i test_data/rv52_a.out)
add_test(g727-40k-33-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv52_a.o test_data/rv52_a.out)

add_test(g727-40k-34 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -dec -law u test_data/rn52_m.i test_data/rn52_m.out)
add_test(g727-40k-34-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn52_m.o test_data/rn52_m.out)

add_test(g727-40k-35 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -dec -law a test_data/rn52_a.i test_data/rn52_a.out)
add_test(g727-40k-35-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn52_a.o test_data/rn52_a.out)

add_test(g727-40k-36 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -dec -law u test_data/i40 test_data/ri52_m.out)
add_test(g727-40k-36-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri52_m.o test_data/ri52_m.out)

add_test(g727-40k-37 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -dec -law a test_data/i40 test_data/ri52_a.out)
add_test(g727-40k-37-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri52_a.o test_data/ri52_a.out)

add_test(g727-40k-38 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -dec -law A test_data/rv52_m.i test_data/rv52_c.out)
add_test(g727-40k-38-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv52_c.o test_data/rv52_c.out)

add_test(g727-40k-39 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -dec -law u test_data/rv52_a.i test_data/rv52_x.out)
add_test(g727-40k-39-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv52_x.o test_data/rv52_x.out)

add_test(g727-40k-40 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -dec -law A test_data/rn52_m.i test_data/rn52_c.out)
add_test(g727-40k-40-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn52_c.o test_data/rn52_c.out)

add_test(g727-40k-41 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -dec -law u test_data/rn52_a.i test_data/rn52_x.out)
add_test(g727-40k-41-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn52_x.o test_data/rn52_x.out)

#Verification tests: 32k
add_test(g727-32k-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -enc -law u test_data/ovr.m test_data/rv44_m.iad)
add_test(g727-32k-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv44_m.i test_data/rv44_m.iad)

add_test(g727-32k-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -enc -law u test_data/nrm.m test_data/rn44_m.iad)
add_test(g727-32k-2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn44_m.i test_data/rn44_m.iad)

add_test(g727-32k-3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -enc -law A test_data/ovr.a test_data/rv44_a.iad)
add_test(g727-32k-3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv44_a.i test_data/rv44_a.iad)

add_test(g727-32k-4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -enc -law A test_data/nrm.a test_data/rn44_a.iad)
add_test(g727-32k-4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn44_a.i test_data/rn44_a.iad)

add_test(g727-32k-5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -dec -law u test_data/rv44_m.i test_data/rv44_m.out)
add_test(g727-32k-5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv44_m.o test_data/rv44_m.out)

add_test(g727-32k-6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -dec -law a test_data/rv44_a.i test_data/rv44_a.out)
add_test(g727-32k-6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv44_a.o test_data/rv44_a.out)

add_test(g727-32k-7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -dec -law u test_data/rn44_m.i test_data/rn44_m.out)
add_test(g727-32k-7-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn44_m.o test_data/rn44_m.out)

add_test(g727-32k-8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -dec -law a test_data/rn44_a.i test_data/rn44_a.out)
add_test(g727-32k-8-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn44_a.o test_data/rn44_a.out)

add_test(g727-32k-9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -dec -law u test_data/i32 test_data/ri44_m.out)
add_test(g727-32k-9-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri44_m.o test_data/ri44_m.out)

add_test(g727-32k-10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -dec -law A test_data/i32 test_data/ri44_a.out)
add_test(g727-32k-11-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri44_a.o test_data/ri44_a.out)

add_test(g727-32k-11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -dec -law A test_data/rv44_m.i test_data/rv44_c.out)
add_test(g727-32k-11-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv44_c.o test_data/rv44_c.out)

add_test(g727-32k-12 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -dec -law u test_data/rv44_a.i test_data/rv44_x.out)
add_test(g727-32k-12-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv44_x.o test_data/rv44_x.out)

add_test(g727-32k-13 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -dec -law A test_data/rn44_m.i test_data/rn44_c.out)
add_test(g727-32k-13-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn44_c.o test_data/rn44_c.out)

add_test(g727-32k-14 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -dec -law u test_data/rn44_a.i test_data/rn44_x.out)
add_test(g727-32k-14-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn44_x.o test_data/rn44_x.out)

add_test(g727-32k-15 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -enc -law u test_data/ovr.m test_data/rv43_m.iad)
add_test(g727-32k-15-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv43_m.i test_data/rv43_m.iad)

add_test(g727-32k-16 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -enc -law u test_data/nrm.m test_data/rn43_m.iad)
add_test(g727-32k-16-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn43_m.i test_data/rn43_m.iad)

add_test(g727-32k-17 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -enc -law A test_data/ovr.a test_data/rv43_a.iad)
add_test(g727-32k-17-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv43_a.i test_data/rv43_a.iad)

add_test(g727-32k-18 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -enc -law A test_data/nrm.a test_data/rn43_a.iad)
add_test(g727-32k-18-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn43_a.i test_data/rn43_a.iad)

add_test(g727-32k-19 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -dec -law u test_data/rv43_m.i test_data/rv43_m.out)
add_test(g727-32k-19-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv43_m.o test_data/rv43_m.out)

add_test(g727-32k-20 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -dec -law a test_data/rv43_a.i test_data/rv43_a.out)
add_test(g727-32k-20-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv43_a.o test_data/rv43_a.out)

add_test(g727-32k-20 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -dec -law u test_data/rn43_m.i test_data/rn43_m.out)
add_test(g727-32k-20-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn43_m.o test_data/rn43_m.out)

add_test(g727-32k-21 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -dec -law a test_data/rn43_a.i test_data/rn43_a.out)
add_test(g727-32k-21-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn43_a.o test_data/rn43_a.out)

add_test(g727-32k-22 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -dec -law u test_data/i32 test_data/ri43_m.out)
add_test(g727-32k-22-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri43_m.o test_data/ri43_m.out)

add_test(g727-32k-23 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -dec -law a test_data/i32 test_data/ri43_a.out)
add_test(g727-32k-23-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri43_a.o test_data/ri43_a.out)

add_test(g727-32k-24 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -dec -law A test_data/rv43_m.i test_data/rv43_c.out)
add_test(g727-32k-24-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv43_c.o test_data/rv43_c.out)

add_test(g727-32k-25 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -dec -law u test_data/rv43_a.i test_data/rv43_x.out)
add_test(g727-32k-25-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv43_x.o test_data/rv43_x.out)

add_test(g727-32k-26 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -dec -law A test_data/rn43_m.i test_data/rn43_c.out)
add_test(g727-32k-26-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn43_c.o test_data/rn43_c.out)

add_test(g727-32k-27 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -dec -law u test_data/rn43_a.i test_data/rn43_x.out)
add_test(g727-32k-27-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn43_x.o test_data/rn43_x.out)

add_test(g727-32k-28 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -enc -law u test_data/ovr.m test_data/rv42_m.iad)
add_test(g727-32k-28-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv42_m.i test_data/rv42_m.iad)

add_test(g727-32k-29 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -enc -law u test_data/nrm.m test_data/rn42_m.iad)
add_test(g727-32k-29-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn42_m.i test_data/rn42_m.iad)

add_test(g727-32k-30 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -enc -law A test_data/ovr.a test_data/rv42_a.iad)
add_test(g727-32k-30-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv42_a.i test_data/rv42_a.iad)

add_test(g727-32k-31 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -enc -law A test_data/nrm.a test_data/rn42_a.iad)
add_test(g727-32k-31-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn42_a.i test_data/rn42_a.iad)

add_test(g727-32k-32 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -dec -law u test_data/rv42_m.i test_data/rv42_m.out)
add_test(g727-32k-32-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv42_m.o test_data/rv42_m.out)

add_test(g727-32k-33 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -dec -law a test_data/rv42_a.i test_data/rv42_a.out)
add_test(g727-32k-33-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv42_a.o test_data/rv42_a.out)

add_test(g727-32k-34 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -dec -law u test_data/rn42_m.i test_data/rn42_m.out)
add_test(g727-32k-34-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn42_m.o test_data/rn42_m.out)

add_test(g727-32k-35 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -dec -law a test_data/rn42_a.i test_data/rn42_a.out)
add_test(g727-32k-35-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn42_a.o test_data/rn42_a.out)

add_test(g727-32k-36 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -dec -law u test_data/i32 test_data/ri42_m.out)
add_test(g727-32k-36-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri42_m.o test_data/ri42_m.out)

add_test(g727-32k-37 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -dec -law a test_data/i32 test_data/ri42_a.out)
add_test(g727-32k-37-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri42_a.o test_data/ri42_a.out)

add_test(g727-32k-38 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -dec -law A test_data/rv42_m.i test_data/rv42_c.out)
add_test(g727-32k-38-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv42_c.o test_data/rv42_c.out)

add_test(g727-32k-39 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -dec -law u test_data/rv42_a.i test_data/rv42_x.out)
add_test(g727-32k-39-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv42_x.o test_data/rv42_x.out)

add_test(g727-32k-40 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -dec -law A test_data/rn42_m.i test_data/rn42_c.out)
add_test(g727-32k-40-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn42_c.o test_data/rn42_c.out)

add_test(g727-32k-41 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -dec -law u test_data/rn42_a.i test_data/rn42_x.out)
add_test(g727-32k-3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn42_x.o test_data/rn42_x.out)

#Verification tests: 24k
add_test(g727-24k-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -enc -law u test_data/ovr.m test_data/rv33_m.iad)
add_test(g727-24k-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv33_m.i test_data/rv33_m.iad)

add_test(g727-24k-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -enc -law u test_data/nrm.m test_data/rn33_m.iad)
add_test(g727-24k-2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn33_m.i test_data/rn33_m.iad)

add_test(g727-24k-3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -enc -law A test_data/ovr.a test_data/rv33_a.iad)
add_test(g727-24k-3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv33_a.i test_data/rv33_a.iad)

add_test(g727-24k-4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -enc -law A test_data/nrm.a test_data/rn33_a.iad)
add_test(g727-24k-4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn33_a.i test_data/rn33_a.iad)

add_test(g727-24k-5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -dec -law u test_data/rv33_m.i test_data/rv33_m.out)
add_test(g727-24k-5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv33_m.o test_data/rv33_m.out)

add_test(g727-24k-6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -dec -law a test_data/rv33_a.i test_data/rv33_a.out)
add_test(g727-24k-6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv33_a.o test_data/rv33_a.out)

add_test(g727-24k-7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -dec -law u test_data/rn33_m.i test_data/rn33_m.out)
add_test(g727-24k-7-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn33_m.o test_data/rn33_m.out)

add_test(g727-24k-8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -dec -law a test_data/rn33_a.i test_data/rn33_a.out)
add_test(g727-24k-8-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn33_a.o test_data/rn33_a.out)

add_test(g727-24k-9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -dec -law u test_data/i24 test_data/ri33_m.out)
add_test(g727-24k-9-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri33_m.o test_data/ri33_m.out)

add_test(g727-24k-10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -dec -law a test_data/i24 test_data/ri33_a.out)
add_test(g727-24k-10-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri33_a.o test_data/ri33_a.out)

add_test(g727-24k-11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -dec -law A test_data/rv33_m.i test_data/rv33_c.out)
add_test(g727-24k-11-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv33_c.o test_data/rv33_c.out)

add_test(g727-24k-12 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -dec -law u test_data/rv33_a.i test_data/rv33_x.out)
add_test(g727-24k-12-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv33_x.o test_data/rv33_x.out)

add_test(g727-24k-13 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -dec -law A test_data/rn33_m.i test_data/rn33_c.out)
add_test(g727-24k-13-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn33_c.o test_data/rn33_c.out)

add_test(g727-24k-14 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -dec -law u test_data/rn33_a.i test_data/rn33_x.out)
add_test(g727-24k-14-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn33_x.o test_data/rn33_x.out)

add_test(g727-24k-15 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -enc -law u test_data/ovr.m test_data/rv32_m.iad)
add_test(g727-24k-15-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv32_m.i test_data/rv32_m.iad)

add_test(g727-24k-16 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -enc -law u test_data/nrm.m test_data/rn32_m.iad)
add_test(g727-24k-16-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn32_m.i test_data/rn32_m.iad)

add_test(g727-24k-17 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -enc -law A test_data/ovr.a test_data/rv32_a.iad)
add_test(g727-24k-17-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv32_a.i test_data/rv32_a.iad)

add_test(g727-24k-18 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -enc -law A test_data/nrm.a test_data/rn32_a.iad)
add_test(g727-24k-18-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn32_a.i test_data/rn32_a.iad)

add_test(g727-24k-18 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -dec -law u test_data/rv32_m.i test_data/rv32_m.out)
add_test(g727-24k-18-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv32_m.o test_data/rv32_m.out)

add_test(g727-24k-19 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -dec -law a test_data/rv32_a.i test_data/rv32_a.out)
add_test(g727-24k-19-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv32_a.o test_data/rv32_a.out)

add_test(g727-24k-20 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -dec -law u test_data/rn32_m.i test_data/rn32_m.out)
add_test(g727-24k-20-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn32_m.o test_data/rn32_m.out)

add_test(g727-24k-21 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -dec -law a test_data/rn32_a.i test_data/rn32_a.out)
add_test(g727-24k-21-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn32_a.o test_data/rn32_a.out)

add_test(g727-24k-22 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -dec -law u test_data/i24 test_data/ri32_m.out)
add_test(g727-24k-22-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri32_m.o test_data/ri32_m.out)

add_test(g727-24k-23 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -dec -law a test_data/i24 test_data/ri32_a.out)
add_test(g727-24k-23-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri32_a.o test_data/ri32_a.out)

add_test(g727-24k-24 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -dec -law A test_data/rv32_m.i test_data/rv32_c.out)
add_test(g727-24k-24-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv32_c.o test_data/rv32_c.out)

add_test(g727-24k-25 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -dec -law u test_data/rv32_a.i test_data/rv32_x.out)
add_test(g727-24k-25-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv32_x.o test_data/rv32_x.out)

add_test(g727-24k-26 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -dec -law A test_data/rn32_m.i test_data/rn32_c.out)
add_test(g727-24k-26-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn32_c.o test_data/rn32_c.out)

add_test(g727-24k-27 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -dec -law u test_data/rn32_a.i test_data/rn32_x.out)
add_test(g727-24k-27-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn32_x.o test_data/rn32_x.out)

#Verification tests: 16k
add_test(g727-16k-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -enc -law u test_data/ovr.m test_data/rv22_m.iad)
add_test(g727-16k-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv22_m.i test_data/rv22_m.iad)

add_test(g727-16k-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -enc -law u test_data/nrm.m test_data/rn22_m.iad)
add_test(g727-16k-2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn22_m.i test_data/rn22_m.iad)

add_test(g727-16k-3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -enc -law A test_data/ovr.a test_data/rv22_a.iad)
add_test(g727-16k-3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv22_a.i test_data/rv22_a.iad)

add_test(g727-16k-4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -enc -law A test_data/nrm.a test_data/rn22_a.iad)
add_test(g727-16k-4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn22_a.i test_data/rn22_a.iad)

add_test(g727-16k-5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -dec -law u test_data/rv22_m.i test_data/rv22_m.out)
add_test(g727-16k-5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv22_m.o test_data/rv22_m.out)

add_test(g727-16k-6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -dec -law a test_data/rv22_a.i test_data/rv22_a.out)
add_test(g727-16k-6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv22_a.o test_data/rv22_a.out)

add_test(g727-16k-7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -dec -law u test_data/rn22_m.i test_data/rn22_m.out)
add_test(g727-16k-7-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn22_m.o test_data/rn22_m.out)

add_test(g727-16k-8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -dec -law a test_data/rn22_a.i test_data/rn22_a.out)
add_test(g727-16k-8-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn22_a.o test_data/rn22_a.out)

add_test(g727-16k-9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -dec -law u test_data/i16 test_data/ri22_m.out)
add_test(g727-16k-9-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri22_m.o test_data/ri22_m.out)

add_test(g727-16k-10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -dec -law a test_data/i16 test_data/ri22_a.out)
add_test(g727-16k-10-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/ri22_a.o test_data/ri22_a.out)

add_test(g727-16k-11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -dec -law A test_data/rv22_m.i test_data/rv22_c.out)
add_test(g727-16k-11-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv22_c.o test_data/rv22_c.out)

add_test(g727-16k-12 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -dec -law u test_data/rv22_a.i test_data/rv22_x.out)
add_test(g727-16k-12-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv22_x.o test_data/rv22_x.out)

add_test(g727-16k-13 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -dec -law A test_data/rn22_m.i test_data/rn22_c.out)
add_test(g727-16k-13-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn22_c.o test_data/rn22_c.out)

add_test(g727-16k-14 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -dec -law u test_data/rn22_a.i test_data/rn22_x.out)
add_test(g727-16k-14-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn22_x.o test_data/rn22_x.out)

#Verification: encode and decode in one step
add_test(g727-e_d-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -e_d -law A test_data/nrm.a test_data/rn22_a.o-c)
add_test(g727-e_d-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn22_a.o test_data/rn22_a.o-c)

add_test(g727-e_d-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -e_d -law A test_data/ovr.a test_data/rv22_a.o-c)
add_test(g727-e_d-2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv22_a.o test_data/rv22_a.o-c)

add_test(g727-e_d-3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -e_d -law u test_data/nrm.m test_data/rn22_m.o-c)
add_test(g727-e_d-3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn22_m.o test_data/rn22_m.o-c)

add_test(g727-e_d-4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 0 -e_d -law u test_data/ovr.m test_data/rv22_m.o-c)
add_test(g727-e_d-4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv22_m.o test_data/rv22_m.o-c)

add_test(g727-e_d-5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -e_d -law A test_data/nrm.a test_data/rn32_a.o-c)
add_test(g727-e_d-5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn32_a.o test_data/rn32_a.o-c)

add_test(g727-e_d-6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -e_d -law A test_data/ovr.a test_data/rv32_a.o-c)
add_test(g727-e_d-6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv32_a.o test_data/rv32_a.o-c)

add_test(g727-e_d-7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -e_d -law u test_data/nrm.m test_data/rn32_m.o-c)
add_test(g727-e_d-7-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn32_m.o test_data/rn32_m.o-c)

add_test(g727-e_d-8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 1 -e_d -law u test_data/ovr.m test_data/rv32_m.o-c)
add_test(g727-e_d-8-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv32_m.o test_data/rv32_m.o-c)

add_test(g727-e_d-9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -e_d -law A test_data/nrm.a test_data/rn42_a.o-c)
add_test(g727-e_d-9-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn42_a.o test_data/rn42_a.o-c)

add_test(g727-e_d-10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -e_d -law A test_data/ovr.a test_data/rv42_a.o-c)
add_test(g727-e_d-10-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv42_a.o test_data/rv42_a.o-c)

add_test(g727-e_d-11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -e_d -law u test_data/nrm.m test_data/rn42_m.o-c)
add_test(g727-e_d-11-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn42_m.o test_data/rn42_m.o-c)

add_test(g727-e_d-12 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 2 -e_d -law u test_data/ovr.m test_data/rv42_m.o-c)
add_test(g727-e_d-12-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv42_m.o test_data/rv42_m.o-c)

add_test(g727-e_d-13 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -e_d -law A test_data/nrm.a test_data/rn52_a.o-c)
add_test(g727-e_d-13-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn52_a.o test_data/rn52_a.o-c)

add_test(g727-e_d-14 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -e_d -law A test_data/ovr.a test_data/rv52_a.o-c)
add_test(g727-e_d-14-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv52_a.o test_data/rv52_a.o-c)

add_test(g727-e_d-15 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -e_d -law u test_data/nrm.m test_data/rn52_m.o-c)
add_test(g727-e_d-15-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn52_m.o test_data/rn52_m.o-c)

add_test(g727-e_d-16 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 2 -enh 3 -e_d -law u test_data/ovr.m test_data/rv52_m.o-c)
add_test(g727-e_d-16-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv52_m.o test_data/rv52_m.o-c)

add_test(g727-e_d-17 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -e_d -law A test_data/nrm.a test_data/rn33_a.o-c)
add_test(g727-e_d-17-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn33_a.o test_data/rn33_a.o-c)

add_test(g727-e_d-18 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -e_d -law A test_data/ovr.a test_data/rv33_a.o-c)
add_test(g727-e_d-18-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv33_a.o test_data/rv33_a.o-c)

add_test(g727-e_d-19 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -e_d -law u test_data/nrm.m test_data/rn33_m.o-c)
add_test(g727-e_d-19-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn33_m.o test_data/rn33_m.o-c)

add_test(g727-e_d-20 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 0 -e_d -law u test_data/ovr.m test_data/rv33_m.o-c)
add_test(g727-e_d-20-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv33_m.o test_data/rv33_m.o-c)

add_test(g727-e_d-21 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -e_d -law A test_data/nrm.a test_data/rn43_a.o-c)
add_test(g727-e_d-21-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn43_a.o test_data/rn43_a.o-c)

add_test(g727-e_d-22 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -e_d -law A test_data/ovr.a test_data/rv43_a.o-c)
add_test(g727-e_d-22-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv43_a.o test_data/rv43_a.o-c)

add_test(g727-e_d-23 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -e_d -law u test_data/nrm.m test_data/rn43_m.o-c)
add_test(g727-e_d-23-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn43_m.o test_data/rn43_m.o-c)

add_test(g727-e_d-24 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 1 -e_d -law u test_data/ovr.m test_data/rv43_m.o-c)
add_test(g727-e_d-24-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv43_m.o test_data/rv43_m.o-c)

add_test(g727-e_d-25 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -e_d -law A test_data/nrm.a test_data/rn53_a.o-c)
add_test(g727-e_d-25-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn53_a.o test_data/rn53_a.o-c)

add_test(g727-e_d-26 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -e_d -law A test_data/ovr.a test_data/rv53_a.o-c)
add_test(g727-e_d-26-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv53_a.o test_data/rv53_a.o-c)

add_test(g727-e_d-27 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -e_d -law u test_data/nrm.m test_data/rn53_m.o-c)
add_test(g727-e_d-27-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn53_m.o test_data/rn53_m.o-c)

add_test(g727-e_d-28 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 3 -enh 2 -e_d -law u test_data/ovr.m test_data/rv53_m.o-c)
add_test(g727-e_d-28-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv53_m.o test_data/rv53_m.o-c)

add_test(g727-e_d-29 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -e_d -law A test_data/nrm.a test_data/rn44_a.o-c)
add_test(g727-e_d-29-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn44_a.o test_data/rn44_a.o-c)

add_test(g727-e_d-30 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -e_d -law A test_data/ovr.a test_data/rv44_a.o-c)
add_test(g727-e_d-30-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv44_a.o test_data/rv44_a.o-c)

add_test(g727-e_d-31 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -e_d -law u test_data/nrm.m test_data/rn44_m.o-c)
add_test(g727-e_d-31-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn44_m.o test_data/rn44_m.o-c)

add_test(g727-e_d-32 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 0 -e_d -law u test_data/ovr.m test_data/rv44_m.o-c)
add_test(g727-e_d-32-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv44_m.o test_data/rv44_m.o-c)

add_test(g727-e_d-33 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -e_d -law A test_data/nrm.a test_data/rn54_a.o-c)
add_test(g727-e_d-33-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn54_a.o test_data/rn54_a.o-c)

add_test(g727-e_d-34 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -e_d -law A test_data/ovr.a test_data/rv54_a.o-c)
add_test(g727-e_d-34-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv54_a.o test_data/rv54_a.o-c)

add_test(g727-e_d-35 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -e_d -law u test_data/nrm.m test_data/rn54_m.o-c)
add_test(g727-e_d-35-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rn54_m.o test_data/rn54_m.o-c)

add_test(g727-e_d-36 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g727demo -q -core 4 -enh 1 -e_d -law u test_data/ovr.m test_data/rv54_m.o-c)
add_test(g727-e_d-3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q  test_data/rv54_m.o test_data/rv54_m.o-c)
