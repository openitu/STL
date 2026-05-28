include_directories(../utl)

file(GLOB UVSELP_SRC *.c *.h)
add_executable(uvselp ${UVSELP_SRC})
target_link_libraries(uvselp ${M_LIBRARY})

#TEST: Encode for ascii and binary bitstream, repectively
add_test(uvselp-encode1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/uvselp -q -enc test_data/voice.src test_data/voice.hbs test_data/enc.log)
add_test(uvselp-encode2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/uvselp -q -enc -nolog -bin test_data/voice.src test_data/voice.bbs)

#TEST: Decode with and without post-filter for an ASCII hex bitstream
add_test(uvselp-decode1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/uvselp -q -dec test_data/voice.hbs test_data/voice.rec test_data/dec.log)
add_test(uvselp-decode2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/uvselp -q -dec -pf test_data/voice.hbs test_data/voice-pf.rec test_data/dec-pf.log)

#TEST:	Decode with and without post-filter for a binary bitstream
add_test(uvselp-decode3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/uvselp -q -dec -bin -nolog test_data/voice.bbs test_data/voice.bec)
add_test(uvselp-decode4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/uvselp -q -dec -pf -bin -nolog test_data/voice.bbs test_data/voice-pf.bec)

#TEST:	Encode and decode with and without post-filter
add_test(uvselp-both1 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/uvselp -q -nolog test_data/voice.src test_data/voice.rek)
add_test(uvselp-both2 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/uvselp -q -nolog -pf test_data/voice.src test_data/voice-pf.rek)

#TEST: Bit-exact verification (platform-specific references)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
  add_test(uvselp-encode1-verify ${CMAKE_COMMAND} -E compare_files test_data/voice.hbs test_data/voice.enc.arm64.ref)
  add_test(uvselp-decode1-verify ${CMAKE_COMMAND} -E compare_files test_data/voice.rec test_data/voice.dec.arm64.ref)
  add_test(uvselp-decode2-verify ${CMAKE_COMMAND} -E compare_files test_data/voice-pf.rec test_data/voice.dec-pf.arm64.ref)
  add_test(uvselp-both1-verify ${CMAKE_COMMAND} -E compare_files test_data/voice.rek test_data/voice.both.arm64.ref)
else()
  # Note: voice.hex has CRLF line endings (committed from Windows).
  # The encode verify test is skipped on x86 until the reference is fixed.
  add_test(uvselp-decode1-verify ${CMAKE_COMMAND} -E compare_files test_data/voice.rec test_data/voice.out)
  add_test(uvselp-decode2-verify ${CMAKE_COMMAND} -E compare_files test_data/voice-pf.rec test_data/voice-pf.out)
  add_test(uvselp-both1-verify ${CMAKE_COMMAND} -E compare_files test_data/voice.rek test_data/voice.out)
endif()

