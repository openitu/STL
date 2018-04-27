file(GLOB G728FLOAT_SRC *.c)
add_executable(g728 ${G728FLOAT_SRC})
target_compile_definitions(g728 PUBLIC USEDOUBLES=1)
target_link_libraries(g728 ${M_LIBRARY})

#TEST: Encoder
add_test(g728-enc1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little enc ../test_data/in1.bin ../test_data/in1.bin.float.out)
add_test(g728-enc1-verify ${CMAKE_COMMAND} -E compare_files ../test_data/incw1.bin ../test_data/in1.bin.float.out)

add_test(g728-enc2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little enc ../test_data/in2.bin ../test_data/in2.bin.float.out)
add_test(g728-enc2-verify ${CMAKE_COMMAND} -E compare_files ../test_data/incw2.bin ../test_data/in2.bin.float.out)

add_test(g728-enc3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little enc ../test_data/in3.bin ../test_data/in3.bin.float.out)
add_test(g728-enc3-verify ${CMAKE_COMMAND} -E compare_files ../test_data/incw3.bin ../test_data/in3.bin.float.out)

add_test(g728-enc4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little enc ../test_data/in4.bin ../test_data/in4.bin.float.out)
add_test(g728-enc4-verify ${CMAKE_COMMAND} -E compare_files ../test_data/incw4.bin ../test_data/in4.bin.float.out)

add_test(g728-enc5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little enc ../test_data/in5.bin ../test_data/in5.bin.float.out)
add_test(g728-enc5-verify ${CMAKE_COMMAND} -E compare_files ../test_data/incw5.bin ../test_data/in5.bin.float.out)

add_test(g728-enc6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little enc ../test_data/in6.bin ../test_data/in6.bin.float.out)
add_test(g728-enc6-verify ${CMAKE_COMMAND} -E compare_files ../test_data/incw6.bin ../test_data/in6.bin.float.out)

#TEST: Decoder without postfilter
add_test(g728-dec1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little -nopostf dec ../test_data/cw1.bin ../test_data/cw1.bin.float.out)
add_test(g728-dec1-verify ${CMAKE_COMMAND} -E compare_files ../test_data/outa1.bin ../test_data/cw1.bin.float.out)

add_test(g728-dec2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little -nopostf dec ../test_data/cw2.bin ../test_data/cw2.bin.float.out)
add_test(g728-dec2-verify ${CMAKE_COMMAND} -E compare_files ../test_data/outa2.bin ../test_data/cw2.bin.float.out)

add_test(g728-dec3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little -nopostf dec ../test_data/cw3.bin ../test_data/cw3.bin.float.out)
add_test(g728-dec3-verify ${CMAKE_COMMAND} -E compare_files ../test_data/outa3.bin ../test_data/cw3.bin.float.out)

add_test(g728-dec4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little -nopostf dec ../test_data/cw4.bin ../test_data/cw4.bin.float.out)
add_test(g728-dec4-verify ${CMAKE_COMMAND} -E compare_files ../test_data/outa4.bin ../test_data/cw4.bin.float.out)

add_test(g728-dec5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little -nopostf dec ../test_data/cw5.bin ../test_data/cw5.bin.float.out)
add_test(g728-dec5-verify ${CMAKE_COMMAND} -E compare_files ../test_data/outa5.bin ../test_data/cw5.bin.float.out)

add_test(g728-dec6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little -nopostf dec ../test_data/cw6.bin ../test_data/cw6.bin.float.out)
add_test(g728-dec6-verify ${CMAKE_COMMAND} -E compare_files ../test_data/outa6.bin ../test_data/cw6.bin.float.out)


#TEST: Decoder with postfilter
add_test(g728-dec-post ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728 -little dec ../test_data/cw4.bin ../test_data/cw4-post.bin.float.out)
add_test(g728-dec-post-verify ${CMAKE_COMMAND} -E compare_files ../test_data/outb4.bin ../test_data/cw4-post.bin.float.out)

