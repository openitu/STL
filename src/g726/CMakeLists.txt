include_directories(../basop)
include_directories(../eid)
include_directories(../g711)
include_directories(../utl)

add_executable(vbr-g726 vbr-g726.c g726.c ../g711/g711.c)
target_link_libraries(vbr-g726 ${M_LIBRARY})

add_executable(g726demo g726demo.c g726.c)
target_link_libraries(g726demo ${M_LIBRARY})

#Verification: g726demo
#NOTE: results have to be identical to the VBR tests - these are therefore not executed by default.
#VBR-g726 and g726demo share the same verification tests: see vbr-g726-verify.
#To tests g726demo, please disable the vbr-g726-tests (except the verification).
#add_test(g726demo1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a load 16 test_data/nrm.a test_data/nrm.a16 256 1 64)
#add_test(g726demo2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a load 16 test_data/ovr.a test_data/ovr.a16 256 1 8)
#add_test(g726demo3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 16 test_data/rn16fa.i test_data/rn16fa.rec 256 1 64)
#add_test(g726demo4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 16 test_data/rv16fa.i test_data/rv16fa.rec 256 1 8)
#add_test(g726demo5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 16 test_data/rn16fa.i test_data/rn16fx.rec 256 1 64)
#add_test(g726demo6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 16 test_data/rv16fa.i test_data/rv16fx.rec 256 1 8)
#add_test(g726demo7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u load 16 test_data/nrm.m test_data/nrm.m16 256 1 64)
#add_test(g726demo8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u load 16 test_data/ovr.m test_data/ovr.m16 256 1 8)
#add_test(g726demo9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 16 test_data/rn16fm.i test_data/rn16fm.rec 256 1 64)
#add_test(g726demo10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 16 test_data/rv16fm.i test_data/rv16fm.rec 256 1 8)
#add_test(g726demo11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 16 test_data/rn16fm.i test_data/rn16fc.rec 256 1 64)
#add_test(g726demo12 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 16 test_data/rv16fm.i test_data/rv16fc.rec 256 1 8)
#add_test(g726demo13 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 16 test_data/i16 test_data/ri16fa.rec 256 1 64)
#add_test(g726demo14 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 16 test_data/i16 test_data/ri16fm.rec 256 1 64)
#add_test(g726demo15 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a load 24 test_data/nrm.a test_data/nrm.a24 256 1 64)
#add_test(g726demo16 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a load 24 test_data/ovr.a test_data/ovr.a24 256 1 8)
#add_test(g726demo17 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 24 test_data/rn24fa.i test_data/rn24fa.rec 256 1 64)
#add_test(g726demo18 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 24 test_data/rv24fa.i test_data/rv24fa.rec 256 1 8)
#add_test(g726demo18 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 24 test_data/rn24fa.i test_data/rn24fx.rec 256 1 64)
#add_test(g726demo19 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 24 test_data/rv24fa.i test_data/rv24fx.rec 256 1 8)
#add_test(g726demo20 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u load 24 test_data/nrm.m test_data/nrm.m24 256 1 64)
#add_test(g726demo21 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u load 24 test_data/ovr.m test_data/ovr.m24 256 1 8)
#add_test(g726demo22 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 24 test_data/rn24fm.i test_data/rn24fm.rec 256 1 64)
#add_test(g726demo23 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 24 test_data/rv24fm.i test_data/rv24fm.rec 256 1 8)
#add_test(g726demo24 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 24 test_data/rn24fm.i test_data/rn24fc.rec 256 1 64)
#add_test(g726demo25 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 24 test_data/rv24fm.i test_data/rv24fc.rec 256 1 8)
#add_test(g726demo26 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 24 test_data/i24 test_data/ri24fa.rec 256 1 64)
#add_test(g726demo27 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 24 test_data/i24 test_data/ri24fm.rec 256 1 64)
#add_test(g726demo28 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a load 40 test_data/nrm.a test_data/nrm.a40 256 1 64)
#add_test(g726demo29 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a load 40 test_data/ovr.a test_data/ovr.a40 256 1 8)
#add_test(g726demo30 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 40 test_data/rn40fa.i test_data/rn40fa.rec 256 1 64)
#add_test(g726demo31 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 40 test_data/rv40fa.i test_data/rv40fa.rec 256 1 8)
#add_test(g726demo32 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 40 test_data/rn40fa.i test_data/rn40fx.rec 256 1 64)
#add_test(g726demo33 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 40 test_data/rv40fa.i test_data/rv40fx.rec 256 1 8)
#add_test(g726demo34 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u load 40 test_data/nrm.m test_data/nrm.m40 256 1 64)
#add_test(g726demo35 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u load 40 test_data/ovr.m test_data/ovr.m40 256 1 8)
#add_test(g726demo36 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 40 test_data/rn40fm.i test_data/rn40fm.rec 256 1 64)
#add_test(g726demo37 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 40 test_data/rv40fm.i test_data/rv40fm.rec 256 1 8)
#add_test(g726demo38 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 40 test_data/rn40fm.i test_data/rn40fc.rec 256 1 64)
#add_test(g726demo39 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 40 test_data/rv40fm.i test_data/rv40fc.rec 256 1 8)
#add_test(g726demo40 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 40 test_data/i40 test_data/ri40fa.rec 256 1 64)
#add_test(g726demo41 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 40 test_data/i40 test_data/ri40fm.rec 256 1 64)
#add_test(g726demo42 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a load 32 test_data/nrm.a test_data/nrm.a32 256 1 64)
#add_test(g726demo43 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a load 32 test_data/ovr.a test_data/ovr.a32 256 1 8)
#add_test(g726demo44 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 32 test_data/rn32fa.i test_data/rn32fa.rec 256 1 64)
#add_test(g726demo45 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 32 test_data/rv32fa.i test_data/rv32fa.rec 256 1 8)
#add_test(g726demo46 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 32 test_data/rn32fa.i test_data/rn32fx.rec 256 1 64)
#add_test(g726demo47 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 32 test_data/rv32fa.i test_data/rv32fx.rec 256 1 8)
#add_test(g726demo48 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u load 32 test_data/nrm.m test_data/nrm.m32 256 1 64)
#add_test(g726demo49 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u load 32 test_data/ovr.m test_data/ovr.m32 256 1 8)
#add_test(g726demo50 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 32 test_data/rn32fm.i test_data/rn32fm.rec 256 1 64)
#add_test(g726demo51 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 32 test_data/rv32fm.i test_data/rv32fm.rec 256 1 8)
#add_test(g726demo52 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 32 test_data/rn32fm.i test_data/rn32fc.rec 256 1 64)
#add_test(g726demo53 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 32 test_data/rv32fm.i test_data/rv32fc.rec 256 1 8)
#add_test(g726demo54 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q a adlo 32 test_data/i32 test_data/ri32fa.rec 256 1 64)
#add_test(g726demo55 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g726demo -q u adlo 32 test_data/i32 test_data/ri32fm.rec 256 1 64)

#Verification: vbr-g726
add_test(g726-vbr1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law A -rate 16-24-32-40-32-24 test_data/voice.src test_data/voicvbra.tst)
add_test(g726-vbr1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/voicvbra.tst test_data/voicevbr.arf)

add_test(g726-vbr2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law l -rate 16-24-32-40-32-24 test_data/voice.src test_data/voicvbrl.tst)
add_test(g726-vbr2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/voicvbrl.tst test_data/voicevbr.lrf)

add_test(g726-vbr3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -rate 16-24-32-40-32-24 test_data/voice.src test_data/voicvbru.tst)
add_test(g726-vbr3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/voicvbru.tst test_data/voicevbr.urf)

add_test(g726-vbr5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -enc -rate 16 test_data/nrm.a test_data/nrm.a16 16 1 1024)
add_test(g726-vbr5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn16fa.i test_data/nrm.a16 256 1 64)

add_test(g726-vbr6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -enc -rate 16 test_data/ovr.a test_data/ovr.a16 16 1 128)
add_test(g726-vbr6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv16fa.i test_data/ovr.a16 256 1 8)

add_test(g726-vbr7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 16 test_data/rn16fa.i test_data/rn16fa.rec 16 1 1024)
add_test(g726-vbr7-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn16fa.o test_data/rn16fa.rec 256 1 64)

add_test(g726-vbr8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 16 test_data/rv16fa.i test_data/rv16fa.rec 16 1 128)
add_test(g726-vbr8-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv16fa.o test_data/rv16fa.rec 256 1 8)

add_test(g726-vbr9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 16 test_data/rn16fa.i test_data/rn16fx.rec 16 1 1024)
add_test(g726-vbr9-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn16fx.o test_data/rn16fx.rec 256 1 64)

add_test(g726-vbr10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 16 test_data/rv16fa.i test_data/rv16fx.rec 16 1 128)
add_test(g726-vbr10-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv16fx.o test_data/rv16fx.rec 256 1 8)

add_test(g726-vbr11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -enc -rate 16 test_data/nrm.m test_data/nrm.m16 16 1 1024)
add_test(g726-vbr11-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn16fm.i test_data/nrm.m16 256 1 64)

add_test(g726-vbr12 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -enc -rate 16 test_data/ovr.m test_data/ovr.m16 16 1 128)
add_test(g726-vbr12-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv16fm.i test_data/ovr.m16 256 1 8)

add_test(g726-vbr13 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 16 test_data/rn16fm.i test_data/rn16fm.rec 16 1 1024)
add_test(g726-vbr13-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn16fm.o test_data/rn16fm.rec 256 1 64)

add_test(g726-vbr14 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 16 test_data/rv16fm.i test_data/rv16fm.rec 16 1 128)
add_test(g726-vbr14-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv16fm.o test_data/rv16fm.rec 256 1 8)

add_test(g726-vbr15 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 16 test_data/rn16fm.i test_data/rn16fc.rec 16 1 1024)
add_test(g726-vbr15-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn16fc.o test_data/rn16fc.rec 256 1 64)

add_test(g726-vbr16 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 16 test_data/rv16fm.i test_data/rv16fc.rec 16 1 128)
add_test(g726-vbr16-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv16fc.o test_data/rv16fc.rec 256 1 8)

add_test(g726-vbr17 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 16 test_data/i16 test_data/ri16fa.rec 16 1 1024)
add_test(g726-vbr17-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/ri16fa.o test_data/ri16fa.rec 256 1 64)

add_test(g726-vbr18 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 16 test_data/i16 test_data/ri16fm.rec 16 1 1024)
add_test(g726-vbr18-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/ri16fm.o test_data/ri16fm.rec 256 1 64)

add_test(g726-vbr19 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -enc -rate 24 test_data/nrm.a test_data/nrm.a24 16 1 1024)
add_test(g726-vbr19-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn24fa.i test_data/nrm.a24 256 1 64)

add_test(g726-vbr20 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -enc -rate 24 test_data/ovr.a test_data/ovr.a24 16 1 128)
add_test(g726-vbr20-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv24fa.i test_data/ovr.a24 256 1 8)

add_test(g726-vbr21 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 24 test_data/rn24fa.i test_data/rn24fa.rec 16 1 1024)
add_test(g726-vbr21-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn24fa.o test_data/rn24fa.rec 256 1 64)

add_test(g726-vbr22 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 24 test_data/rv24fa.i test_data/rv24fa.rec 16 1 128)
add_test(g726-vbr22-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv24fa.o test_data/rv24fa.rec 256 1 8)

add_test(g726-vbr23 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 24 test_data/rn24fa.i test_data/rn24fx.rec 16 1 1024)
add_test(g726-vbr23-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn24fx.o test_data/rn24fx.rec 256 1 64)

add_test(g726-vbr24 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 24 test_data/rv24fa.i test_data/rv24fx.rec 16 1 128)
add_test(g726-vbr24-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv24fx.o test_data/rv24fx.rec 256 1 8)

add_test(g726-vbr25 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -enc -rate 24 test_data/nrm.m test_data/nrm.m24 16 1 1024)
add_test(g726-vbr25-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn24fm.i test_data/nrm.m24 256 1 64)

add_test(g726-vbr26 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -enc -rate 24 test_data/ovr.m test_data/ovr.m24 16 1 128)
add_test(g726-vbr26-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv24fm.i test_data/ovr.m24 256 1 8)

add_test(g726-vbr27 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 24 test_data/rn24fm.i test_data/rn24fm.rec 16 1 1024)
add_test(g726-vbr27-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn24fm.o test_data/rn24fm.rec 256 1 64)

add_test(g726-vbr28 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 24 test_data/rv24fm.i test_data/rv24fm.rec 16 1 128)
add_test(g726-vbr28-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv24fm.o test_data/rv24fm.rec 256 1 8)

add_test(g726-vbr39 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 24 test_data/rn24fm.i test_data/rn24fc.rec 16 1 1024)
add_test(g726-vbr39-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn24fc.o test_data/rn24fc.rec 256 1 64)

add_test(g726-vbr30 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 24 test_data/rv24fm.i test_data/rv24fc.rec 16 1 128)
add_test(g726-vbr30-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv24fc.o test_data/rv24fc.rec 256 1 8)

add_test(g726-vbr31 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 24 test_data/i24 test_data/ri24fa.rec 16 1 1024)
add_test(g726-vbr31-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/ri24fa.o test_data/ri24fa.rec 256 1 64)

add_test(g726-vbr32 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 24 test_data/i24 test_data/ri24fm.rec 16 1 1024)
add_test(g726-vbr32-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/ri24fm.o test_data/ri24fm.rec 256 1 64)

add_test(g726-vbr33 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -enc -rate 32 test_data/nrm.a test_data/nrm.a32 16 1 1024)
add_test(g726-vbr33-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn32fa.i test_data/nrm.a32 256 1 64)

add_test(g726-vbr34 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -enc -rate 32 test_data/ovr.a test_data/ovr.a32 16 1 128)
add_test(g726-vbr34-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv32fa.i test_data/ovr.a32 256 1 8)

add_test(g726-vbr35 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 32 test_data/rn32fa.i test_data/rn32fa.rec 16 1 1024)
add_test(g726-vbr35-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn32fa.o test_data/rn32fa.rec 256 1 64)

add_test(g726-vbr36 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 32 test_data/rv32fa.i test_data/rv32fa.rec 16 1 128)
add_test(g726-vbr36-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv32fa.o test_data/rv32fa.rec 256 1 8)

add_test(g726-vbr37 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 32 test_data/rn32fa.i test_data/rn32fx.rec 16 1 1024)
add_test(g726-vbr37-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn32fx.o test_data/rn32fx.rec 256 1 64)

add_test(g726-vbr38 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 32 test_data/rv32fa.i test_data/rv32fx.rec 16 1 128)
add_test(g726-vbr38-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv32fx.o test_data/rv32fx.rec 256 1 8)

add_test(g726-vbr39 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -enc -rate 32 test_data/nrm.m test_data/nrm.m32 16 1 1024)
add_test(g726-vbr39-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn32fm.i test_data/nrm.m32 256 1 64)

add_test(g726-vbr40 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -enc -rate 32 test_data/ovr.m test_data/ovr.m32 16 1 128)
add_test(g726-vbr40-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv32fm.i test_data/ovr.m32 256 1 8)

add_test(g726-vbr41 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 32 test_data/rn32fm.i test_data/rn32fm.rec 16 1 1024)
add_test(g726-vbr41-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn32fm.o test_data/rn32fm.rec 256 1 64)

add_test(g726-vbr42 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 32 test_data/rv32fm.i test_data/rv32fm.rec 16 1 128)
add_test(g726-vbr42-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv32fm.o test_data/rv32fm.rec 256 1 8)

add_test(g726-vbr43 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 32 test_data/rn32fm.i test_data/rn32fc.rec 16 1 1024)
add_test(g726-vbr43-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn32fc.o test_data/rn32fc.rec 256 1 64)

add_test(g726-vbr44 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 32 test_data/rv32fm.i test_data/rv32fc.rec 16 1 128)
add_test(g726-vbr44-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv32fc.o test_data/rv32fc.rec 256 1 8)

add_test(g726-vbr45 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 32 test_data/i32 test_data/ri32fa.rec 16 1 1024)
add_test(g726-vbr45-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/ri32fa.o test_data/ri32fa.rec 256 1 64)

add_test(g726-vbr46 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 32 test_data/i32 test_data/ri32fm.rec 16 1 1024)
add_test(g726-vbr46-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/ri32fm.o test_data/ri32fm.rec 256 1 64)

add_test(g726-vbr47 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -enc -rate 40 test_data/nrm.a test_data/nrm.a40 16 1 1024)
add_test(g726-vbr47-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn40fa.i test_data/nrm.a40 256 1 64)

add_test(g726-vbr48 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -enc -rate 40 test_data/ovr.a test_data/ovr.a40 16 1 128)
add_test(g726-vbr48-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv40fa.i test_data/ovr.a40 256 1 8)

add_test(g726-vbr49 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 40 test_data/rn40fa.i test_data/rn40fa.rec 16 1 1024)
add_test(g726-vbr49-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn40fa.o test_data/rn40fa.rec 256 1 64)

add_test(g726-vbr50 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 40 test_data/rv40fa.i test_data/rv40fa.rec 16 1 128)
add_test(g726-vbr50-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv40fa.o test_data/rv40fa.rec 256 1 8)

add_test(g726-vbr51 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 40 test_data/rn40fa.i test_data/rn40fx.rec 16 1 1024)
add_test(g726-vbr51-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn40fx.o test_data/rn40fx.rec 256 1 64)

add_test(g726-vbr52 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 40 test_data/rv40fa.i test_data/rv40fx.rec 16 1 128)
add_test(g726-vbr52-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv40fx.o test_data/rv40fx.rec 256 1 8)

add_test(g726-vbr53 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -enc -rate 40 test_data/nrm.m test_data/nrm.m40 16 1 1024)
add_test(g726-vbr53-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn40fm.i test_data/nrm.m40 256 1 64)

add_test(g726-vbr54 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -enc -rate 40 test_data/ovr.m test_data/ovr.m40 16 1 128)
add_test(g726-vbr54-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv40fm.i test_data/ovr.m40 256 1 8)

add_test(g726-vbr55 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 40 test_data/rn40fm.i test_data/rn40fm.rec 16 1 1024)
add_test(g726-vbr55-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn40fm.o test_data/rn40fm.rec 256 1 64)

add_test(g726-vbr56 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 40 test_data/rv40fm.i test_data/rv40fm.rec 16 1 128)
add_test(g726-vbr56-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv40fm.o test_data/rv40fm.rec 256 1 8)

add_test(g726-vbr57 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 40 test_data/rn40fm.i test_data/rn40fc.rec 16 1 1024)
add_test(g726-vbr57-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rn40fc.o test_data/rn40fc.rec 256 1 64)

add_test(g726-vbr58 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 40 test_data/rv40fm.i test_data/rv40fc.rec 16 1 128)
add_test(g726-vbr58-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/rv40fc.o test_data/rv40fc.rec 256 1 8)

add_test(g726-vbr59 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law a -dec -rate 40 test_data/i40 test_data/ri40fa.rec 16 1 1024)
add_test(g726-vbr59-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/ri40fa.o test_data/ri40fa.rec 256 1 64)

add_test(g726-vbr60 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -dec -rate 40 test_data/i40 test_data/ri40fm.rec 16 1 1024)
add_test(g726-vbr60-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/ri40fm.o test_data/ri40fm.rec 256 1 64)
