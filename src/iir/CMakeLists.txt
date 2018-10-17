include_directories(../utl)

add_executable(pcmdemo pcmdemo.c iir-g712.c iir-lib.c ../utl/ugst-utl.c)
target_link_libraries(pcmdemo ${M_LIBRARY})

add_executable(cirsdemo cirsdemo.c iir-irs.c iir-lib.c ../utl/ugst-utl.c)
target_link_libraries(cirsdemo ${M_LIBRARY})

add_executable(c712demo c712demo.c cascg712.c iir-lib.c ../utl/ugst-utl.c)
target_link_libraries(c712demo ${M_LIBRARY})

add_test(pcmdemo1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pcmdemo test_data/test.src test_data/testg712.100 1_1 0 0)
add_test(pcmdemo1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/testpcm1.ref test_data/testg712.100 256 1 30)

add_test(pcmdemo2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pcmdemo test_data/test.src test_data/testg712.u00 1_2 0 0)
add_test(pcmdemo2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/testpcmu.ref test_data/testg712.u00 256 1 30)

add_test(pcmdemo3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pcmdemo test_data/test.src test_data/testg712.d00 2_1 0 0)
add_test(pcmdemo3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/testpcmd.ref test_data/testg712.d00 256 1 30)

add_test(pcmdemo4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pcmdemo test_data/test.src test_data/testg712.010 0 1_1 0)
add_test(pcmdemo4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/testpcm1.ref test_data/testg712.010 256 1 30)

add_test(pcmdemo5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pcmdemo test_data/test.src test_data/testg712.0u0 0 1_2 0)
add_test(pcmdemo5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/testpcmu.ref test_data/testg712.0u0 256 1 30)

add_test(pcmdemo6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pcmdemo test_data/test.src test_data/testg712.0d0 0 2_1 0)
add_test(pcmdemo6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/testpcmd.ref test_data/testg712.0d0 256 1 30)

add_test(pcmdemo7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pcmdemo test_data/test.src test_data/testg712.001 0 0 1_1)
add_test(pcmdemo7-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/testpcm1.ref test_data/testg712.001 256 1 30)

add_test(pcmdemo8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pcmdemo test_data/test.src test_data/testg712.00u 0 0 1_2)
add_test(pcmdemo8-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/testpcmu.ref test_data/testg712.00u 256 1 30)

add_test(pcmdemo9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/pcmdemo test_data/test.src test_data/testg712.00d 0 0 2_1)
add_test(pcmdemo9-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/testpcmd.ref test_data/testg712.00d 256 1 30)

add_test(cirsdemo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cirsdemo test_data/test.src test_data/iir-irs.flt)
add_test(cirsdemo-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/iir-irs.ref test_data/iir-irs.flt 256 1 30)

add_test(c712demo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/c712demo test_data/test.src test_data/cascg712.flt)
add_test(c712demo-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/cascg712.ref test_data/cascg712.flt 256 1 30)

