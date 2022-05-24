include_directories(../utl)

add_executable(p50fbmnru bg_mnr70_src.c filtering_routines.c)
target_link_libraries(p50fbmnru ${M_LIBRARY})

#TEST: P50 FB MNRU
add_test(p50fbmnru-1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/p50fbmnru test_data/P501_D_AM_fm_FB_48k.pcm test_data/P501_D_AM_fm_FB_48k_Q20.pcm 20 M)
add_test(p50fbmnru-1-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/P501_D_AM_fm_FB_48k_Q20.ref test_data/P501_D_AM_fm_FB_48k_Q20.pcm)

add_test(p50fbmnru-2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/p50fbmnru test_data/P501_D_AM_fm_FB_48k.pcm test_data/P501_D_AM_fm_FB_48k_Q10.pcm 10 M)
add_test(p50fbmnru-2-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/P501_D_AM_fm_FB_48k_Q10.ref test_data/P501_D_AM_fm_FB_48k_Q10.pcm)

add_test(p50fbmnru-3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/p50fbmnru test_data/P501_D_AM_fm_FB_48k.pcm test_data/P501_D_AM_fm_FB_48k_Q20_noise.pcm 20 N)
add_test(p50fbmnru-3-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/P501_D_AM_fm_FB_48k_Q20_noise.ref test_data/P501_D_AM_fm_FB_48k_Q20_noise.pcm)

add_test(p50fbmnru-4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/p50fbmnru test_data/P501_D_AM_fm_FB_48k.pcm test_data/P501_D_AM_fm_FB_48k_Q10_noise.pcm 10 N)
add_test(p50fbmnru-4-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/P501_D_AM_fm_FB_48k_Q10_noise.ref test_data/P501_D_AM_fm_FB_48k_Q10_noise.pcm)

add_test(p50fbmnru-5 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/p50fbmnru test_data/P501_D_EN_fm_SWB_48k.pcm test_data/P501_D_EN_fm_SWB_48k_Q20.pcm 20 M)
add_test(p50fbmnru-5-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/P501_D_EN_fm_SWB_48k_Q20.ref test_data/P501_D_EN_fm_SWB_48k_Q20.pcm)

add_test(p50fbmnru-6 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/p50fbmnru test_data/P501_D_EN_fm_SWB_48k.pcm test_data/P501_D_EN_fm_SWB_48k_Q10.pcm 10 M)
add_test(p50fbmnru-6-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/P501_D_EN_fm_SWB_48k_Q10.ref test_data/P501_D_EN_fm_SWB_48k_Q10.pcm)

add_test(p50fbmnru-7 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/p50fbmnru test_data/P501_D_EN_fm_SWB_48k.pcm test_data/P501_D_EN_fm_SWB_48k_Q20_noise.pcm 20 M)
add_test(p50fbmnru-7-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/P501_D_EN_fm_SWB_48k_Q20_noise.ref test_data/P501_D_EN_fm_SWB_48k_Q20_noise.pcm)

add_test(p50fbmnru-8 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/p50fbmnru test_data/P501_D_EN_fm_SWB_48k.pcm test_data/P501_D_EN_fm_SWB_48k_Q10_noise.pcm 10 M)
add_test(p50fbmnru-8-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/P501_D_EN_fm_SWB_48k_Q10_noise.ref test_data/P501_D_EN_fm_SWB_48k_Q10_noise.pcm)
