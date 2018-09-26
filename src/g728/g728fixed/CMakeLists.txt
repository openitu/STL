file(GLOB G728FP_SRC *.c)
add_executable(g728fp ${G728FP_SRC})
target_link_libraries(g728fp ${M_LIBRARY})

#TEST: Encoder
add_test(g728fp-enc1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little enc ../test_data/in1.bin ../test_data/in1.bin.fp.out)
add_test(g728fp-enc1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/incw1g.bin ../test_data/in1.bin.fp.out)

add_test(g728fp-enc2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little enc ../test_data/in2.bin ../test_data/in2.bin.fp.out)
add_test(g728fp-enc2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/incw2g.bin ../test_data/in2.bin.fp.out)

add_test(g728fp-enc3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little enc ../test_data/in3.bin ../test_data/in3.bin.fp.out)
add_test(g728fp-enc3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/incw3g.bin ../test_data/in3.bin.fp.out)

add_test(g728fp-enc4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little enc ../test_data/in4.bin ../test_data/in4.bin.fp.out)
add_test(g728fp-enc4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/incw4g.bin ../test_data/in4.bin.fp.out)

add_test(g728fp-enc5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little enc ../test_data/in5.bin ../test_data/in5.bin.fp.out)
add_test(g728fp-enc5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/incw5g.bin ../test_data/in5.bin.fp.out)

add_test(g728fp-enc6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little enc ../test_data/in6.bin ../test_data/in6.bin.fp.out)
add_test(g728fp-enc6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/incw6g.bin ../test_data/in6.bin.fp.out)

#TEST: Decoder without postfilter
add_test(g728fp-dec1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little -nopostf dec ../test_data/cw1.bin ../test_data/cw1.bin.fp.out)
add_test(g728fp-dec1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/outa1g.bin ../test_data/cw1.bin.fp.out)

add_test(g728fp-dec2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little -nopostf dec ../test_data/cw2.bin ../test_data/cw2.bin.fp.out)
add_test(g728fp-dec2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/outa2g.bin ../test_data/cw2.bin.fp.out)

add_test(g728fp-dec3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little -nopostf dec ../test_data/cw3.bin ../test_data/cw3.bin.fp.out)
add_test(g728fp-dec3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/outa3g.bin ../test_data/cw3.bin.fp.out)

add_test(g728fp-dec4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little -nopostf dec ../test_data/cw4.bin ../test_data/cw4.bin.fp.out)
add_test(g728fp-dec4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/outa4g.bin ../test_data/cw4.bin.fp.out)

add_test(g728fp-dec5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little -nopostf dec ../test_data/cw5.bin ../test_data/cw5.bin.fp.out)
add_test(g728fp-dec5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/outa5g.bin ../test_data/cw5.bin.fp.out)

add_test(g728fp-dec6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little -nopostf dec ../test_data/cw6.bin ../test_data/cw6.bin.fp.out)
add_test(g728fp-dec6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/outa6g.bin ../test_data/cw6.bin.fp.out)

#TEST: Decoder with postfilter
add_test(g728fp-dec-post ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/g728fp -little dec ../test_data/cw4.bin ../test_data/cw4-post.bin.fp.out)
add_test(g728fp-dec-post-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q ../test_data/outb4g.bin ../test_data/cw4-post.bin.fp.out)

