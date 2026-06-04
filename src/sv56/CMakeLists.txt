include_directories(../g711)
include_directories(../utl)

add_executable(sv56demo sv56demo.c  sv-p56.c ../utl/ugst-utl.c ../utl/wav_io.c)
target_link_libraries(sv56demo ${M_LIBRARY})

add_executable(actlev actlevel.c  sv-p56.c ../utl/ugst-utl.c ../utl/wav_io.c)
target_link_libraries(actlev ${M_LIBRARY})

add_test(sv56demo1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56demo -q test_data/voice.src test_data/voice.prc 256 1 0 -30)
set_tests_properties(sv56demo1 PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo1-verify ${CMAKE_COMMAND} -E compare_files test_data/voice.nrm test_data/voice.prc)
set_tests_properties(sv56demo1-verify PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56demo -q -rms test_data/voice.src test_data/voice.rms 256 1 0 -30)
set_tests_properties(sv56demo2 PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo2-verify ${CMAKE_COMMAND} -E compare_files test_data/voice.ltl test_data/voice.rms)
set_tests_properties(sv56demo2-verify PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/actlev -q test_data/voice.src test_data/voice.nrm test_data/voice.prc test_data/voice.ltl test_data/voice.rms)
set_tests_properties(sv56demo3 PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(sv56test sv56test.c sv-p56.c)
target_link_libraries(sv56test ${M_LIBRARY})

add_executable(sv56-convert sv56-convert.c)
target_link_libraries(sv56-convert ${M_LIBRARY})

add_test(sv56test-16bit ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56test 16 test_data/voice.src test_data/voice_16bit.out)
set_tests_properties(sv56test-16bit PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56test-16bit-verify ${CMAKE_COMMAND} -E compare_files test_data/voice_16bit.ref test_data/voice_16bit.out)
set_tests_properties(sv56test-16bit-verify PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56test-24bit ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56test 24 test_data/voice_24.src test_data/voice_24bit.out)
set_tests_properties(sv56test-24bit PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56test-24bit-verify ${CMAKE_COMMAND} -E compare_files test_data/voice_24bit.ref test_data/voice_24bit.out)
set_tests_properties(sv56test-24bit-verify PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56test-32bit ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56test 32 test_data/voice_32.src test_data/voice_32bit.out)
set_tests_properties(sv56test-32bit PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56test-32bit-verify ${CMAKE_COMMAND} -E compare_files test_data/voice_32bit.ref test_data/voice_32bit.out)
set_tests_properties(sv56test-32bit-verify PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# WAV integration tests
add_test(sv56demo-wav-24bit-normal ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56demo -q test_data/speech_normal_24bit.wav test_data/speech_normal_24bit.out)
set_tests_properties(sv56demo-wav-24bit-normal PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-24bit-normal-verify ${CMAKE_COMMAND} -E compare_files test_data/speech_normal_24bit.ref test_data/speech_normal_24bit.out)
set_tests_properties(sv56demo-wav-24bit-normal-verify PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-32bit ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56demo -q test_data/speech_normal_32bit.wav test_data/speech_normal_32bit.out)
set_tests_properties(sv56demo-wav-32bit PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-32bit-verify ${CMAKE_COMMAND} -E compare_files test_data/speech_normal_32bit.ref test_data/speech_normal_32bit.out)
set_tests_properties(sv56demo-wav-32bit-verify PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-24bit ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56demo -q test_data/speech_quiet_24bit.wav test_data/speech_quiet_24bit.out)
set_tests_properties(sv56demo-wav-24bit PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-24bit-verify ${CMAKE_COMMAND} -E compare_files test_data/speech_quiet_24bit.ref test_data/speech_quiet_24bit.out)
set_tests_properties(sv56demo-wav-24bit-verify PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-float ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56demo -q test_data/speech_quiet_32bit_float.wav test_data/speech_quiet_32bit_float.out)
set_tests_properties(sv56demo-wav-float PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-float-verify ${CMAKE_COMMAND} -E compare_files test_data/speech_quiet_32bit_float.ref test_data/speech_quiet_32bit_float.out)
set_tests_properties(sv56demo-wav-float-verify PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-vquiet-32bit ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56demo -q test_data/speech_vquiet_32bit.wav test_data/speech_vquiet_32bit.out)
set_tests_properties(sv56demo-wav-vquiet-32bit PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-vquiet-32bit-verify ${CMAKE_COMMAND} -E compare_files test_data/speech_vquiet_32bit.ref test_data/speech_vquiet_32bit.out)
set_tests_properties(sv56demo-wav-vquiet-32bit-verify PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-vquiet-16bit-adc ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56demo -q -bits 16 test_data/speech_vquiet_32bit.wav test_data/speech_vquiet_32bit_16bit.out)
set_tests_properties(sv56demo-wav-vquiet-16bit-adc PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-vquiet-16bit-adc-verify ${CMAKE_COMMAND} -E compare_files test_data/speech_vquiet_32bit_16bit.ref test_data/speech_vquiet_32bit_16bit.out)
set_tests_properties(sv56demo-wav-vquiet-16bit-adc-verify PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# WAV text output (statistics) verification tests
add_test(sv56demo-wav-24bit-normal-log ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56demo -q -log test_data/speech_normal_24bit.log test_data/speech_normal_24bit.wav test_data/speech_normal_24bit.out)
set_tests_properties(sv56demo-wav-24bit-normal-log PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-24bit-normal-log-verify ${CMAKE_COMMAND}
  -DGOT=${CMAKE_CURRENT_SOURCE_DIR}/test_data/speech_normal_24bit.log
  -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/test_data/speech_normal_24bit.log.ref
  -DLABEL=sv56demo-wav-24bit-normal-log
  -P ${CMAKE_SOURCE_DIR}/cmake/CompareTextFiles.cmake)

add_test(sv56demo-wav-32bit-log ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56demo -q -log test_data/speech_normal_32bit.log test_data/speech_normal_32bit.wav test_data/speech_normal_32bit.out)
set_tests_properties(sv56demo-wav-32bit-log PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-32bit-log-verify ${CMAKE_COMMAND}
  -DGOT=${CMAKE_CURRENT_SOURCE_DIR}/test_data/speech_normal_32bit.log
  -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/test_data/speech_normal_32bit.log.ref
  -DLABEL=sv56demo-wav-32bit-log
  -P ${CMAKE_SOURCE_DIR}/cmake/CompareTextFiles.cmake)

add_test(sv56demo-wav-vquiet-32bit-log ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56demo -q -log test_data/speech_vquiet_32bit.log test_data/speech_vquiet_32bit.wav test_data/speech_vquiet_32bit.out)
set_tests_properties(sv56demo-wav-vquiet-32bit-log PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-vquiet-32bit-log-verify ${CMAKE_COMMAND}
  -DGOT=${CMAKE_CURRENT_SOURCE_DIR}/test_data/speech_vquiet_32bit.log
  -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/test_data/speech_vquiet_32bit.log.ref
  -DLABEL=sv56demo-wav-vquiet-32bit-log
  -P ${CMAKE_SOURCE_DIR}/cmake/CompareTextFiles.cmake)

add_test(sv56demo-wav-vquiet-16bit-adc-log ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56demo -q -log test_data/speech_vquiet_32bit_16bit.log -bits 16 test_data/speech_vquiet_32bit.wav test_data/speech_vquiet_32bit_16bit.out)
set_tests_properties(sv56demo-wav-vquiet-16bit-adc-log PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56demo-wav-vquiet-16bit-adc-log-verify ${CMAKE_COMMAND}
  -DGOT=${CMAKE_CURRENT_SOURCE_DIR}/test_data/speech_vquiet_32bit_16bit.log
  -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/test_data/speech_vquiet_32bit_16bit.log.ref
  -DLABEL=sv56demo-wav-vquiet-16bit-adc-log
  -P ${CMAKE_SOURCE_DIR}/cmake/CompareTextFiles.cmake)
