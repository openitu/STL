include_directories(../iir)
include_directories(../utl)


add_executable(filter filter.c fir-dsm.c fir-flat.c fir-irs.c fir-lib.c fir-pso.c fir-tia.c fir-hirs.c fir-wb.c fir-msin.c fir-LP.c ../iir/iir-lib.c ../iir/iir-g712.c ../iir/iir-dir.c ../iir/iir-flat.c ../utl/ugst-utl.c)
target_link_libraries(filter ${M_LIBRARY})

add_executable(flt fltresp.c fir-dsm.c fir-flat.c fir-irs.c fir-lib.c fir-pso.c fir-tia.c fir-hirs.c fir-wb.c fir-msin.c fir-LP.c ../iir/iir-lib.c ../iir/iir-g712.c ../iir/iir-dir.c ../iir/iir-flat.c)
target_link_libraries(flt ${M_LIBRARY})

add_executable(firdemo firdemo.c fir-dsm.c fir-flat.c fir-irs.c fir-lib.c fir-pso.c fir-tia.c fir-hirs.c fir-wb.c fir-msin.c fir-LP.c ../iir/iir-lib.c ../iir/iir-g712.c ../iir/iir-dir.c ../iir/iir-flat.c ../utl/ugst-utl.c)
target_link_libraries(firdemo ${M_LIBRARY})

#Test: FIR
add_test(firdemo1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test001.hqp       8 0  0  0  0  0)
add_test(firdemo1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test001.hqp test_data/test001.ref)

add_test(firdemo2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test002.hqp      16 0  0  0  0  0)
add_test(firdemo2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test002.hqp test_data/test002.ref)

add_test(firdemo3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test003.hqp       0 1 0  0  0  0)
add_test(firdemo3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test003.hqp test_data/test003.ref)

add_test(firdemo4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test004.hqp       0 0  2  0  0  0)
add_test(firdemo4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test004.hqp test_data/test004.ref)

add_test(firdemo5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test005.hqp       0 0  3  0  0  0)
add_test(firdemo5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test005.hqp test_data/test005.ref)

add_test(firdemo6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test006.hqp       0 0  0  2  0  0)
add_test(firdemo6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test006.hqp test_data/test004.ref)

add_test(firdemo7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test007.hqp       0 0  0  3  0  0)
add_test(firdemo7-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test007.hqp test_data/test005.ref)

add_test(firdemo8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test008.hqp       0 0  0  0  2  0)
add_test(firdemo8-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test008.hqp test_data/test008.ref)

add_test(firdemo9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test009.hqp       0 0  0  0  3  0)
add_test(firdemo9-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test009.hqp test_data/test009.ref)

add_test(firdemo10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test010.hqp       0 0  0  0  0  2)
add_test(firdemo10-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test010.hqp test_data/test008.ref)

add_test(firdemo11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test011.hqp       0 0  0  0  0  3)
add_test(firdemo11-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test011.hqp test_data/test009.ref)

add_test(firdemo12 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test012.hqp       0 0  2  3  2  3)
add_test(firdemo12-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test012.hqp test_data/test012.ref)

add_test(firdemo13 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test013.hqp       0 0  3  2  3  2)
add_test(firdemo13-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test013.hqp test_data/test012.ref)

add_test(firdemo14 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test014.hqp       8 0  2  3  2  3)
add_test(firdemo14-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test014.hqp test_data/test014.ref)

add_test(firdemo15 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test015.hqp      16 0  2  3  2  3)
add_test(firdemo15-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test015.hqp test_data/test015.ref)

add_test(firdemo16 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test016.hqp       8 1 2  3  2  3)
add_test(firdemo16-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test016.hqp test_data/test014.ref)

add_test(firdemo17 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test017.hqp      16 1 2  3  2  3)
add_test(firdemo17-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test017.hqp test_data/test017.ref)

add_test(firdemo18 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q -mod test_data/test.src test_data/test018.hqp 16 0  0  0  0  0)
add_test(firdemo18-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test018.hqp test_data/test018.ref)

add_test(firdemo19 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q -mod test_data/test.src test_data/test019.hqp 48 0  0  0  0  0)
add_test(firdemo19-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test019.hqp test_data/test019.ref)

add_test(firdemo20 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test020.hqp       0 0 -2  0  0  0)
add_test(firdemo20-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test020.hqp test_data/test020.ref)

add_test(firdemo21 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test021.hqp       0 0  0 -2  0  0)
add_test(firdemo21-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test021.hqp test_data/test020.ref)

add_test(firdemo22 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test022.hqp       0 0  0  0 -2  0)
add_test(firdemo22-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test022.hqp test_data/test022.ref)

add_test(firdemo23 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q test_data/test.src test_data/test023.hqp       0 0  0  0  0 -2)
add_test(firdemo23-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test023.hqp test_data/test022.ref)

add_test(firdemo24 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/firdemo -q -ht test_data/test.src test_data/test024.hqp  16 0  0  0  0  0)
add_test(firdemo24-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/test024.hqp test_data/test024.ref)

#Test: filter
add_test(filter1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q IRS8 test_data/test.src test_data/irs8.flt)
add_test(filter1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/irs8.flt   test_data/test001.ref)

add_test(filter2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q IRS16 test_data/test.src test_data/irs16.flt)
add_test(filter2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/irs16.flt   test_data/test002.ref)

add_test(filter3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -mod IRS16 test_data/test.src test_data/irs16-m.flt)
add_test(filter3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/irs16-m.flt test_data/test018.ref)

add_test(filter4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q IRS48 test_data/test.src test_data/irs48.flt)
add_test(filter4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/irs48.flt   test_data/test019.ref)

add_test(filter5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q RXIRS8 test_data/test.src test_data/rxmirs8.flt)
add_test(filter5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/rxmirs8.flt test_data/rxmirs8.ref)

add_test(filter6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q RXIRS16 test_data/test.src test_data/rxmirs16.flt)
add_test(filter6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/rxmirs16.flt test_data/rxmirs16.ref)

add_test(filter7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q HIRS16 test_data/test.src test_data/ht-irs16.flt)
add_test(filter7-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/ht-irs16.flt test_data/test024.ref)

add_test(filter8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q DSM test_data/test.src test_data/dsm.flt)
add_test(filter8-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/dsm.flt     test_data/test003.ref)

add_test(filter9 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q PSO test_data/test.src test_data/pso.flt)
add_test(filter9-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/pso.flt     test_data/test-pso.ref)

add_test(filter10 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q GSM1 test_data/test.src test_data/tst-msin.flt)
add_test(filter10-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/tst-msin.flt	test_data/testmsin.ref)

add_test(filter11 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -up HQ2 test_data/test.src test_data/hq2-up.flt)
add_test(filter11-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/hq2-up.flt  test_data/test004.ref)

add_test(filter12 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -down HQ2 test_data/test.src test_data/hq2-dw.flt)
add_test(filter12-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/hq2-dw.flt  test_data/test008.ref)

add_test(filter13 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -up HQ3 test_data/test.src test_data/hq3-up.flt)
add_test(filter13-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/hq3-up.flt  test_data/test005.ref)

add_test(filter14 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -down HQ3 test_data/test.src test_data/hq3-dw.flt)
add_test(filter14-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/hq3-dw.flt  test_data/test009.ref)

add_test(filter15 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -up FLAT test_data/test.src test_data/flat-up.flt)
add_test(filter15-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/flat-up.flt test_data/test020.ref)

add_test(filter16 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -down FLAT test_data/test.src test_data/flat-dw.flt)
add_test(filter16-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/flat-dw.flt test_data/test022.ref)

add_test(filter17 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q FLAT1 test_data/test.src test_data/testfla1.flt)
add_test(filter17-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/testfla1.flt	test_data/testfla1.ref)

add_test(filter18 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -up PCM test_data/test.src test_data/pcm-up.flt)
add_test(filter18-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/pcm-up.flt  test_data/testpcmu.ref)

add_test(filter19 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -down PCM test_data/test.src test_data/pcm-dw.flt)
add_test(filter19-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/pcm-dw.flt  test_data/testpcmd.ref)

add_test(filter20 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q PCM1 test_data/test.src test_data/pcm1.flt)
add_test(filter20-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -equiv 1 test_data/pcm1.flt    test_data/testpcm1.ref)

add_test(filter21 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -up iflat test_data/test.src test_data/test-cas.flt)
add_test(filter21-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q test_data/test-cas.flt test_data/test-cas.ref)

add_test(filter22 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -async iflat test_data/test.src test_data/test-asy.flt)
add_test(filter22-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q test_data/test-asy.flt test_data/test-asy.ref)

add_test(filter23 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -async -delay 37 iflat test_data/test.src test_data/tst-asyd.flt)
add_test(filter23-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -delay 37 test_data/tst-asyd.flt test_data/test-asy.flt)

add_test(filter24 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -async -delay -37 iflat test_data/test.src test_data/tst-asys.flt)
add_test(filter24-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q -delay -37 test_data/tst-asys.flt test_data/test-asy.flt)

add_test(filter25 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q -down iflat test_data/test.src test_data/test-sac.flt)
add_test(filter25-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q test_data/test-sac.flt test_data/test-sac.ref)

add_test(filter26 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q p341 test_data/test.src test_data/testp341.flt)
add_test(filter26-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q test_data/testp341.flt test_data/testp341.ref)

add_test(filter27 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/filter -q 5kbp test_data/test.src test_data/test5kbp.flt)
add_test(filter27-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/signal-diff -q test_data/test5kbp.flt test_data/test5kbp.ref)
