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

add_test(sv56test-16bit-verify ${CMAKE_COMMAND}
  -DGOT=${CMAKE_CURRENT_SOURCE_DIR}/test_data/voice_16bit.out
  -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/test_data/voice_16bit.ref
  -DLABEL=sv56test-16bit
  -P ${CMAKE_SOURCE_DIR}/cmake/CompareTextFiles.cmake)

add_test(sv56test-24bit ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56test 24 test_data/voice_24.src test_data/voice_24bit.out)
set_tests_properties(sv56test-24bit PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56test-24bit-verify ${CMAKE_COMMAND}
  -DGOT=${CMAKE_CURRENT_SOURCE_DIR}/test_data/voice_24bit.out
  -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/test_data/voice_24bit.ref
  -DLABEL=sv56test-24bit
  -P ${CMAKE_SOURCE_DIR}/cmake/CompareTextFiles.cmake)

add_test(sv56test-32bit ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/sv56test 32 test_data/voice_32.src test_data/voice_32bit.out)
set_tests_properties(sv56test-32bit PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_test(sv56test-32bit-verify ${CMAKE_COMMAND}
  -DGOT=${CMAKE_CURRENT_SOURCE_DIR}/test_data/voice_32bit.out
  -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/test_data/voice_32bit.ref
  -DLABEL=sv56test-32bit
  -P ${CMAKE_SOURCE_DIR}/cmake/CompareTextFiles.cmake)
