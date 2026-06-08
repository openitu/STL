include_directories(../utl)

add_executable(asc2bin asc2bin.c)
target_link_libraries(asc2bin ${M_LIBRARY})

add_executable(astrip astrip.c ../utl/wav_io.c)
target_link_libraries(astrip ${M_LIBRARY})

add_executable(bin2asc bin2asc.c)
target_link_libraries(bin2asc ${M_LIBRARY})

add_executable(cf compfile.c)
target_link_libraries(cf ${M_LIBRARY})

add_executable(concat concat.c)

add_executable(df dumpfile.c)
target_link_libraries(df ${M_LIBRARY})

add_executable(endian endian.c)

add_executable(fdelay fdelay.c ../utl/wav_io.c)
target_link_libraries(fdelay ${M_LIBRARY})

add_executable(measure measure.c ../utl/wav_io.c)
target_link_libraries(measure ${M_LIBRARY})

add_executable(oper oper.c ../utl/wav_io.c)
target_link_libraries(oper ${M_LIBRARY})

add_executable(sb sb.c)

add_executable(sine sine.c ../utl/wav_io.c)
target_link_libraries(sine ${M_LIBRARY})

add_executable(chr2sh chr2sh.c)

add_executable(sh2chr sh2chr.c)

add_executable(crc getcrc32.c)

add_executable(random random.c)
target_link_libraries(random ${M_LIBRARY})

add_test(astrip1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/astrip -q -smooth -wlen 145 -sample test_data/cftest1.dat test_data/astrip.smp)
add_test(astrip1-verify ${CMAKE_COMMAND} -E compare_files test_data/astrip.smp test_data/astrip.ref)

add_test(astrip2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/astrip -q -smooth -wlen 145 test_data/cftest1.dat test_data/astrip.blk 160)
add_test(astrip2-verify ${CMAKE_COMMAND} -E compare_files test_data/astrip.blk test_data/astrip.ref)

add_test(concat1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/concat -f -undo test_data/undo.txt test_data/concat1.txt test_data/concat2.txt test_data/concat3.tst)
add_test(concat1-verify ${CMAKE_COMMAND} -E compare_files test_data/concat3.tst test_data/concat3.txt)

set(UNSUP_TEST_DATA ${CMAKE_CURRENT_SOURCE_DIR}/test_data)
set(UNSUP_TEST_OUT ${CMAKE_CURRENT_BINARY_DIR}/test_out)
file(MAKE_DIRECTORY ${UNSUP_TEST_OUT})

add_test(sb1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sb ${UNSUP_TEST_DATA}/bigend.src ${UNSUP_TEST_OUT}/bytswpd.tst)
set_tests_properties(sb1 PROPERTIES FIXTURES_SETUP SB1_DONE RESOURCE_LOCK SB_FILE)
add_test(sb1-verify ${CMAKE_COMMAND} -E compare_files ${UNSUP_TEST_OUT}/bytswpd.tst ${UNSUP_TEST_DATA}/litend.src)
set_tests_properties(sb1-verify PROPERTIES FIXTURES_REQUIRED SB1_DONE FIXTURES_SETUP SB1_VERIFIED RESOURCE_LOCK SB_FILE)

# Debug: check file exists before sb2
add_test(NAME sb2-debug COMMAND ${CMAKE_COMMAND} -E sha256sum ${UNSUP_TEST_OUT}/bytswpd.tst)
set_tests_properties(sb2-debug PROPERTIES FIXTURES_REQUIRED SB1_VERIFIED FIXTURES_SETUP SB2_DEBUG_DONE RESOURCE_LOCK SB_FILE)

add_test(sb2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sb -over ${UNSUP_TEST_OUT}/bytswpd.tst)
set_tests_properties(sb2 PROPERTIES FIXTURES_REQUIRED SB2_DEBUG_DONE FIXTURES_SETUP SB2_DONE RESOURCE_LOCK SB_FILE)
add_test(sb2-verify ${CMAKE_COMMAND} -E compare_files ${UNSUP_TEST_OUT}/bytswpd.tst ${UNSUP_TEST_DATA}/bigend.src)
set_tests_properties(sb2-verify PROPERTIES FIXTURES_REQUIRED SB2_DONE RESOURCE_LOCK SB_FILE)

add_test(fdelay1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fdelay test_data/bigend.src test_data/delaydft.tst)
add_test(fdelay1-verify ${CMAKE_COMMAND} -E compare_files test_data/delaydft.tst test_data/delaydft.ref)

add_test(fdelay2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fdelay -file test_data/litend.src test_data/bigend.src test_data/delayfil.tst)
add_test(fdelay2-verify ${CMAKE_COMMAND} -E compare_files test_data/delayfil.tst test_data/delayfil.ref)

add_test(fdelay3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fdelay -law a test_data/litend.src test_data/delay-a.tst)
add_test(fdelay3-verify ${CMAKE_COMMAND} -E compare_files test_data/delay-a.tst test_data/delay-a.ref)

add_test(fdelay4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fdelay -delay 57 -law u test_data/litend.src test_data/delay-u.tst)
add_test(fdelay4-verify ${CMAKE_COMMAND} -E compare_files test_data/delay-u.tst test_data/delay-u.ref)

add_test(fdelay5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fdelay -dec 15 test_data/litend.src test_data/delay-15.tst)
add_test(fdelay5-verify ${CMAKE_COMMAND} -E compare_files test_data/delay-15.tst test_data/delay-15.ref)

add_test(fdelay6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fdelay -hex 0xF test_data/litend.src test_data/delay-0f.tst)
add_test(fdelay6-verify ${CMAKE_COMMAND} -E compare_files test_data/delay-0f.tst test_data/delay-15.ref)

add_test(random1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/random -s 12345 -r 10 20 -n 5)
set_tests_properties(random1 PROPERTIES PASS_REGULAR_EXPRESSION "12 19 12 14 14")

add_test(random2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/random -s 42 alpha bravo charlie delta echo)
set_tests_properties(random2 PROPERTIES PASS_REGULAR_EXPRESSION "^alpha ")

add_test(random3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/random -s 42 -d 3 -n 3 alpha bravo charlie delta echo)
set_tests_properties(random3 PROPERTIES PASS_REGULAR_EXPRESSION "bravo delta charlie")
