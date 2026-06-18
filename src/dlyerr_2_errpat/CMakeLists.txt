add_executable(dlyerr_2_errpat dlyerr_2_errpat.c)

# default text format, constant delay
add_test(dlyerr_2_errpat1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dlyerr_2_errpat
         -i test_data/profile.dat -o test_data/t1_text.proc -L 30 -d 40)
add_test(dlyerr_2_errpat1-verify ${CMAKE_COMMAND} -E compare_files
         test_data/t1_text.ref test_data/t1_text.proc)

# LF text format
add_test(dlyerr_2_errpat2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dlyerr_2_errpat
         -i test_data/profile.dat -o test_data/t2_lf.proc -L 30 -d 40 -c)
add_test(dlyerr_2_errpat2-verify ${CMAKE_COMMAND} -E compare_files
         test_data/t2_lf.ref test_data/t2_lf.proc)

# word G.192 ASCII format with frame aggregation
add_test(dlyerr_2_errpat3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dlyerr_2_errpat
         -i test_data/profile.dat -o test_data/t3_word.proc -L 30 -d 40 -w -f 2)
add_test(dlyerr_2_errpat3-verify ${CMAKE_COMMAND} -E compare_files
         test_data/t3_word.ref test_data/t3_word.proc)

# late-loss-rate driven threshold
add_test(dlyerr_2_errpat4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dlyerr_2_errpat
         -i test_data/profile.dat -o test_data/t4_lateloss.proc -L 30 -l 5.0)
add_test(dlyerr_2_errpat4-verify ${CMAKE_COMMAND} -E compare_files
         test_data/t4_lateloss.ref test_data/t4_lateloss.proc)
