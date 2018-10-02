include_directories(../eid)
include_directories(../utl)

add_executable(g711iplc g711iplc.c plcferio.c g711iplc.c lowcfe.c ../eid/softbit.c)
target_compile_definitions(g711iplc PUBLIC USEDOUBLES=1)
target_link_libraries(g711iplc ${M_LIBRARY})

add_executable(asc2g192 asc2g192.c)
target_link_libraries(asc2g192 ${M_LIBRARY})

add_test(g711iplc1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711iplc -stats test_data/fe10.g192 test_data/f2.le test_data/f2_10_c.raw)
add_test(g711iplc1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/f2_10.raw test_data/f2_10_c.raw)

add_test(g711iplc2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711iplc -noplc -stats test_data/fe10.g192 test_data/f2.le test_data/f2_10m_c.raw)
add_test(g711iplc2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/f2_10m.raw test_data/f2_10m_c.raw)

add_test(g711iplc3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711iplc -stats test_data/fe10_2.g192 test_data/f2.le test_data/f2_10_2_c.raw)
add_test(g711iplc3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/f2_10_2.raw test_data/f2_10_2_c.raw)

add_test(g711iplc4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711iplc -noplc -stats test_data/fe10_2.g192 test_data/f2.le test_data/f2_10_2m_c.raw)
add_test(g711iplc4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/f2_10_2m.raw test_data/f2_10_2m_c.raw)
