include_directories(../utl)

add_executable(asc2bin asc2bin.c)
target_link_libraries(asc2bin ${M_LIBRARY})

add_executable(astrip astrip.c)
target_link_libraries(astrip ${M_LIBRARY})

add_executable(bin2asc bin2asc.c)
target_link_libraries(bin2asc ${M_LIBRARY})

add_executable(cf compfile.c)
target_link_libraries(cf ${M_LIBRARY})

add_executable(concat concat.c)

add_executable(df dumpfile.c)
target_link_libraries(df ${M_LIBRARY})

add_executable(endian endian.c)

add_executable(fdelay fdelay.c)
target_link_libraries(fdelay ${M_LIBRARY})

add_executable(measure measure.c)
target_link_libraries(measure ${M_LIBRARY})

add_executable(oper oper.c)
target_link_libraries(oper ${M_LIBRARY})

add_executable(sb sb.c)

add_executable(sine sine.c)
target_link_libraries(sine ${M_LIBRARY})

add_executable(chr2sh chr2sh.c)

add_executable(sh2chr sh2chr.c)

add_executable(crc getcrc32.c)

add_executable(xencode xencode.c)

add_executable(xdecode xdecode.c)

add_test(astrip1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/astrip -q -smooth -wlen 145 -sample test_data/cftest1.dat test_data/astrip.smp)
add_test(astrip1-verify ${CMAKE_COMMAND} -E compare_files test_data/astrip.smp test_data/astrip.ref)

add_test(astrip2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/astrip -q -smooth -wlen 145 test_data/cftest1.dat test_data/astrip.blk 160)
add_test(astrip2-verify ${CMAKE_COMMAND} -E compare_files test_data/astrip.blk test_data/astrip.ref)

add_test(concat1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/concat -f -undo test_data/undo.txt test_data/concat1.txt test_data/concat2.txt test_data/concat3.tst)
add_test(concat1-verify ${CMAKE_COMMAND} -E compare_files test_data/concat3.tst test_data/concat3.txt)

add_test(sb1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sb test_data/bigend.src test_data/bytswpd.tst)
add_test(sb1-verify ${CMAKE_COMMAND} -E compare_files test_data/bytswpd.tst test_data/litend.src)

add_test(sb2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sb -over test_data/bytswpd.tst)
add_test(sb2-verify ${CMAKE_COMMAND} -E compare_files test_data/bytswpd.tst test_data/bigend.src)

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

add_test(NAME xdecode1 COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xdecode voice.uue WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_data)
add_test(xdecode1-verify ${CMAKE_COMMAND} -E compare_files test_data/voice.rec test_data/voice.ori)

add_test(NAME xencode1 COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xencode voice.rec v.uue 7 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_data)
add_test(xencode1-verify ${CMAKE_COMMAND} -E compare_files test_data/v.uue test_data/voice.uue)
add_test(xencode2-verify ${CMAKE_COMMAND} -E compare_files test_data/v01.uue test_data/voice01.uue)
