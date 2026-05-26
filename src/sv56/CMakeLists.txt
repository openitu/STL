include_directories(../g711)
include_directories(../utl)

add_executable(sv56demo sv56demo.c  sv-p56.c ../utl/ugst-utl.c)
target_link_libraries(sv56demo ${M_LIBRARY})

add_executable(actlev actlevel.c  sv-p56.c ../utl/ugst-utl.c)
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
