include_directories(../eid)
include_directories(../utl)

add_executable(truncate truncate.c trunc-lib.c ../eid/softbit.c ../utl/ugst-utl.c)
target_link_libraries(truncate ${M_LIBRARY})

add_test(truncate1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/truncate -ib 8000 -b 6000 test_data/bin_bst.test test_data/bin_bst_6k.proc)
add_test(truncate1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/bin_bst_6k.ref test_data/bin_bst_6k.proc)

add_test(truncate2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/truncate -ib 8000 -b 6000 test_data/byte_bst.test test_data/byte_bst_6k.proc)
add_test(truncate2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/byte_bst_6k.ref test_data/byte_bst_6k.proc)

add_test(truncate3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/truncate -b 6000 test_data/byte_sync_bst.test test_data/byte_sync_bst_6k.proc)
add_test(truncate3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf test_data/byte_sync_bst_6k.ref test_data/byte_sync_bst_6k.proc)

add_test(truncate4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/truncate -ib 8000 -b 6000 test_data/g192_bst.test test_data/g192_bst_6k.proc)
add_test(truncate4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/g192_bst_6k.ref test_data/g192_bst_6k.proc)

add_test(truncate5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/truncate -b 6000 test_data/g192_sync_bst.test test_data/g192_sync_bst_6k.proc)
add_test(truncate5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/g192_sync_bst_6k.ref test_data/g192_sync_bst_6k.proc)
