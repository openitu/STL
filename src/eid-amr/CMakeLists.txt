add_executable(eid-amr eid-amr.c)
target_link_libraries(eid-amr ${M_LIBRARY})

add_test(eid-amr ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eid-amr test_data/bin_bst.src test_data/bf06.g192 bin_bst06.proc)

