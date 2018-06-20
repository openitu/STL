file(GLOB BASOP_SRC ../*.c)
list(REMOVE_ITEM BASOP_SRC "${CMAKE_CURRENT_LIST_DIR}/../basop_cnt.c")


file(GLOB BASOP_TEST_SRC ./src/*.c ./test/*.c)
include_directories( ./ ../ ./src  )
add_executable(basop_test ${BASOP_SRC} ${BASOP_TEST_SRC})


target_compile_definitions(basop_test PUBLIC ENH_64_BIT_OPERATOR ENH_32_BIT_OPERATOR COMPLEX_OPERATOR ALLOW_ENH_UL32 IDENTICAL_FLOAT_INP CHECK_SANITY  )

target_link_libraries(basop_test ${M_LIBRARY})


file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/basop_test_out )
add_test( NAME basop_test WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/basop_test_out COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/basop_test )