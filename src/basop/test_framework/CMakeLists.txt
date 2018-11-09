file(GLOB BASOP_SRC ../*.c)
list(REMOVE_ITEM BASOP_SRC "${CMAKE_CURRENT_LIST_DIR}/../basop_cnt.c")

file(GLOB BASOP_TEST_SRC ./src/*.c ./test/*.c)
include_directories( ./ ../ ./src )
add_executable(basop_test ${BASOP_SRC} ${BASOP_TEST_SRC})

target_compile_definitions(basop_test PUBLIC ENH_64_BIT_OPERATOR ENH_32_BIT_OPERATOR COMPLEX_OPERATOR ALLOW_ENH_UL32 IDENTICAL_FLOAT_INP)

target_link_libraries(basop_test ${M_LIBRARY})


file(MAKE_DIRECTORY ./test_data )
add_test( NAME basop_sanity_test    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_data COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/basop_test Test_type=0 )
add_test( NAME basop_precision_vectors WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_data COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/basop_test Test_type=1 )

# List all reference files and compare each of them
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/test/test_ref/*.csv")
foreach(file ${files})

  # Extract basename
  get_filename_component( basename ${file} NAME )

  # Perform diff
  add_test( basop_precision_test_${basename}
    ${CMAKE_COMMAND} -E compare_files
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test_ref/${basename}
    ${CMAKE_CURRENT_SOURCE_DIR}/test_data/${basename}
  )

endforeach()
