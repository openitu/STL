include_directories(../utl)

add_executable(bs-stats bs-stats.c softbit.c)
target_link_libraries(bs-stats ${M_LIBRARY})

add_executable(cvt-head cvt-head.c softbit.c)
target_link_libraries(cvt-head ${M_LIBRARY})

#Example: not compiled by default
#add_executable(eid eid.c softbit.c)
#target_link_libraries(eid ${M_LIBRARY})

add_executable(eid8k eid8k.c eid.c eid_io.c)
target_link_libraries(eid8k ${M_LIBRARY})

add_executable(eiddemo eiddemo.c eid.c)
target_link_libraries(eiddemo ${M_LIBRARY})

add_executable(eid-ev eid-ev.c softbit.c)
target_link_libraries(eid-ev ${M_LIBRARY})

add_executable(eid-int eid-int.c softbit.c)
target_link_libraries(eid-int ${M_LIBRARY})

add_executable(eid-xor eid-xor.c softbit.c)
target_link_libraries(eid-xor ${M_LIBRARY})

add_executable(ep-stats ep-stats.c softbit.c)
target_link_libraries(ep-stats ${M_LIBRARY})

add_executable(gen-patt gen-patt.c eid.c eid_io.c softbit.c)
target_link_libraries(gen-patt ${M_LIBRARY})

add_executable(gen_rate_profile gen_rate_profile.c)
target_link_libraries(gen_rate_profile ${M_LIBRARY})

add_executable(g729e_convert_synch g729e_convert_synch.c softbit.c)
target_link_libraries(g729e_convert_synch ${M_LIBRARY})

add_test(bs-stats ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bs-stats -qq test_data/dummy.bs)

#Test: eiddemo (zero)
add_test(eiddemo1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eiddemo test_data/zero.ser test_data/b3g0f0g0.ser test_data/eiddemo-1.ber test_data/eiddemo-1.fer 0.001 0.00 0.00 0.0)
add_test(eiddemo2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eiddemo test_data/zero.ser test_data/b3g0f0g0.ser test_data/eiddemo-2.ber test_data/eiddemo-2.fer 0.001 0.01 0.00 0.0)
add_test(eiddemo3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eiddemo test_data/zero.ser test_data/b0g0f2g0.ser test_data/eiddemo-3.ber test_data/eiddemo-3.fer 0.000 0.00 0.01 0.0)
add_test(eiddemo4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eiddemo test_data/zero.ser test_data/b0g0f2g5.ser test_data/eiddemo-4.ber test_data/eiddemo-4.fer 0.000 0.00 0.01 0.5)

#Test: measure (zero)
#TODO
#	measure -crc b3g0f0g0.ser >  xxx
#	measure -crc b3g2f0g0.ser >> xxx
#	measure -crc b0g0f2g0.ser >> xxx
#	measure -crc b0g0f2g5.ser >> xxx
#	$(AWK) AWK_CMD = 'BEGIN{OFS="\t"};{print $$4,$$6,$$8,$$10,$$12,$$14,$$16,$$18}' xxx

#Test: eid8k
add_test(eid8k1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid8k test_data/patr10-2.ser r 100  1 test_data/eid8k-1.sta .01)
add_test(eid8k2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid8k test_data/patf10-2.ser f 1000 1 test_data/eid8k-2.sta .01)
add_test(eid8k3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid8k test_data/patb1pct.ser b 1000 1 test_data/eid8k-3.sta 1)
add_test(eid8k4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid8k test_data/patb3pct.ser b 1000 1 test_data/eid8k-4.sta 3)
add_test(eid8k5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid8k test_data/patb5pct.ser b 1000 1 test_data/eid8k-5.sta 5)

#Test: measure (8k)
#TODO
#measure -crc patf10-2.ser > xxx
#measure -crc patr10-2.ser >> xxx
#measure -crc patb1pct.ser >> xxx
#measure -crc patb3pct.ser >> xxx
#measure -crc patb5pct.ser >> xxx
#	$(AWK) AWK_CMD = 'BEGIN{OFS="\t"};{print $$4,$$6,$$8,$$10,$$12,$$14,$$16,$$18}' xxx

#Test: gen-patt
add_test(gen-patt1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -g192 test_data/patr10-2.ser r 10000 1 test_data/gen-patt1.sta .01)
add_test(gen-patt2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -g192 test_data/patr10-2.ser f 10000 1 test_data/gen-patt2.sta .01)
add_test(gen-patt3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -g192 test_data/patr10-2.ser b 10000 1 test_data/gen-patt3.sta .01)
add_test(gen-patt4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -g192 test_data/patr10-2.ser b 10000 1 test_data/gen-patt4.sta .03)
add_test(gen-patt5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -g192 test_data/patb5pct.ser b 10000 1 test_data/gen-patt5.sta .05)

add_test(gen-patt6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -byte test_data/patr10-2.ser r 10000 1 test_data/gen-patt6.sta .01)
add_test(gen-patt7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -byte test_data/patr10-2.ser f 10000 1 test_data/gen-patt7.sta .01)
add_test(gen-patt8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -byte test_data/patr10-2.ser b 10000 1 test_data/gen-patt8.sta .01)
add_test(gen-patt9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -byte test_data/patr10-2.ser b 10000 1 test_data/gen-patt9.sta .03)
add_test(gen-patt10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -byte test_data/patb5pct.ser b 10000 1 test_data/gen-patt10.sta .05)

add_test(gen-patt11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -bit test_data/patr10-2.ser r 10000 1 test_data/gen-patt11.sta .01)
add_test(gen-patt12 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -bit test_data/patr10-2.ser f 10000 1 test_data/gen-patt12.sta .01)
add_test(gen-patt13 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -bit test_data/patr10-2.ser b 10000 1 test_data/gen-patt13.sta .01)
add_test(gen-patt14 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -bit test_data/patr10-2.ser b 10000 1 test_data/gen-patt14.sta .03)
add_test(gen-patt15 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -bit test_data/patb5pct.ser b 10000 1 test_data/gen-patt15.sta .05)

add_test(gen-patt16 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -g192 -ber -rate 0.05 -gamma 0.10 test_data/epr05g10.192 r 10000 1)
add_test(gen-patt17 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -byte -ber -rate 0.05 -gamma 0.10 test_data/epr05g10.byt r 10000 1)
add_test(gen-patt18 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -bit  -ber -rate 0.05 -gamma 0.10 test_data/epr05g10.bit r 10000 1)
add_test(gen-patt19 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -g192 -fer -rate 0.05 -gamma 0.10 test_data/epf05g10.192 f 10000 1)
add_test(gen-patt20 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -byte -fer -rate 0.05 -gamma 0.10 test_data/epf05g10.byt f 10000 1)
add_test(gen-patt21 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gen-patt -bit  -fer -rate 0.05 -gamma 0.10 test_data/epf05g10.bit f 10000 1)

#Test: eid-xor
add_test(eid-xor ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid-xor -ber -bs bit -ep g192 test_data/zero.src test_data/epr05g10.192 test_data/z_r05g10.bg1)
add_test(eid-xor ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid-xor -ber -bs bit -ep byte test_data/zero.src test_data/epr05g10.byt test_data/z_r05g10.bby)
add_test(eid-xor ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid-xor -ber -bs bit -ep bit  test_data/zero.src test_data/epr05g10.bit test_data/z_r05g10.bbi)
add_test(eid-xor ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid-xor -fer -bs bit -ep g192 test_data/zero.src test_data/epf05g10.192 test_data/z_f05g10.bg1)
add_test(eid-xor ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid-xor -fer -bs bit -ep byte test_data/zero.src test_data/epf05g10.byt test_data/z_f05g10.bby)
add_test(eid-xor ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid-xor -fer -bs bit -ep bit  test_data/zero.src test_data/epf05g10.bit test_data/z_f05g10.bbi)

