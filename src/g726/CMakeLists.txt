include_directories(../basop)
include_directories(../eid)
include_directories(../g711)
include_directories(../utl)

add_executable(vbr-g726 vbr-g726.c g726.c ../g711/g711.c)
target_link_libraries(vbr-g726 ${M_LIBRARY})

add_executable(g726demo g726demo.c g726.c)
target_link_libraries(g726demo ${M_LIBRARY})

add_test(g726-vbr1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law A -rate 16-24-32-40-32-24 test_data/voice.src test_data/voicvbra.tst)
add_test(g726-vbr1-verify ${CMAKE_COMMAND} -E compare_files test_data/voicvbra.tst test_data/voicevbr.arf)

add_test(g726-vbr2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law l -rate 16-24-32-40-32-24 test_data/voice.src test_data/voicvbrl.tst)
add_test(g726-vbr2-verify ${CMAKE_COMMAND} -E compare_files test_data/voicvbrl.tst test_data/voicevbr.lrf)

add_test(g726-vbr3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vbr-g726 -q -law u -rate 16-24-32-40-32-24 test_data/voice.src test_data/voicvbru.tst)
add_test(g726-vbr3-verify ${CMAKE_COMMAND} -E compare_files test_data/voicvbru.tst test_data/voicevbr.urf)
