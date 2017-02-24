include_directories(../utl)

add_executable(g711demo g711demo.c g711.c)
target_link_libraries(g711demo ${M_LIBRARY})

add_executable(shiftbit shiftbit.c)

add_test(g711demo1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo A lilo test_data/sweep.src test_data/sweep.a 256 1 256)
add_test(g711demo1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sweep.a test_data/sweep-r.a)

add_test(g711demo2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo u lilo test_data/sweep.src test_data/sweep.u 256 1 256)
add_test(g711demo2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sweep.u test_data/sweep-r.u)

add_test(g711demo3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo A lili test_data/sweep.src test_data/sweep.a-a 256 1 256)
add_test(g711demo3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sweep.a-a test_data/sweep-r.a-a)

add_test(g711demo4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo u lili test_data/sweep.src test_data/sweep.u-u 256 1 256)
add_test(g711demo4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sweep.u-u test_data/sweep-r.u-u)

add_test(g711demo5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo A loli test_data/sweep.a test_data/sweep.rea 256 1 256)
add_test(g711demo5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sweep.rea test_data/sweep-r.rea)

add_test(g711demo6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g711demo u loli test_data/sweep.u test_data/sweep.reu 256 1 256)
add_test(g711demo6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sweep.reu test_data/sweep-r.reu)
