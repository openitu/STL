include_directories(../utl)

add_executable(gain_chk gain_chk.c ../utl/wav_io.c)
target_link_libraries(gain_chk ${M_LIBRARY})

set(_gain_chk_testdir "${CMAKE_CURRENT_BINARY_DIR}/gain_chk_ctest_scratch")
file(MAKE_DIRECTORY "${_gain_chk_testdir}")

# Reference PCM: input48c.pcm; processed: syn.{a,b,c,d}.pcm; stderr vs gain_chk_stderr_*.ref
foreach(_case self a b c d)
    add_test(
        NAME gain_chk_${_case}
        COMMAND ${CMAKE_COMMAND}
            -DGAIN_CHK=$<TARGET_FILE:gain_chk>
            -DPCM_DIR=${CMAKE_CURRENT_SOURCE_DIR}/test_data
            -DWORKDIR=${_gain_chk_testdir}
            -DCASE=${_case}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/test_data/gain_chk_ctest.cmake
    )
endforeach()
