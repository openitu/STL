add_executable(eid-amr eid-amr.c)
target_link_libraries(eid-amr ${M_LIBRARY})

add_test(eid-amr1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid-amr test_data/bst475.amr test_data/bf03.g192 test_data/bst475_bf03.tst)
add_test(eid-amr2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid-amr test_data/bst122.amr test_data/bf06.g192 test_data/bst122_bf06.tst)

add_test(eid-amr-verify1 ${CMAKE_COMMAND} -E compare_files test_data/bst475_bf03.amr test_data/bst475_bf03.tst)
add_test(eid-amr-verify2 ${CMAKE_COMMAND} -E compare_files test_data/bst122_bf06.amr test_data/bst122_bf06.tst)

