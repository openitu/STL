# WAV I/O integration tests
# Tests that tools correctly handle WAV input with matching/mismatching parameters
# and that raw PCM input still works (backward compatibility).

set(BIN ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(WAV_TEST_DIR ${CMAKE_CURRENT_BINARY_DIR}/wav_test_data)
file(MAKE_DIRECTORY ${WAV_TEST_DIR})

# --- Generate WAV test files from existing raw PCM test data ---

# G.711 test input: 8000 Hz (narrowband codec)
add_test(NAME wav-gen-g711-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/sweep_8k.wav 8000 1 16 test_data/sweep.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/g711)
add_test(NAME wav-gen-g711-wrong
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/sweep_48k.wav 48000 1 16 test_data/sweep.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/g711)

# SV56 test input: 16000 Hz (default)
add_test(NAME wav-gen-sv56-16k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/voice_16k.wav 16000 1 16 test_data/voice.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/sv56)
add_test(NAME wav-gen-sv56-wrong
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/voice_44k.wav 44100 1 16 test_data/voice.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/sv56)

# MNRU test input: no fixed rate (pass-through)
add_test(NAME wav-gen-mnru-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/sine_8k.wav 8000 1 16 test_data/sine.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/mnru)

# G.726 test input: 8000 Hz
add_test(NAME wav-gen-g726-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/g726_voice_8k.wav 8000 1 16 test_data/voice.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/g726)
add_test(NAME wav-gen-g726-wrong
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/g726_voice_16k.wav 16000 1 16 test_data/voice.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/g726)

# FIR filter test input
add_test(NAME wav-gen-fir-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/fir_test_8k.wav 8000 1 16 test_data/test.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/fir)

# Reverb test input (pass-through, any rate)
add_test(NAME wav-gen-reverb-16k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/reverb_input_16k.wav 16000 1 16 test_data/input.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/reverb)

# Set fixture properties for WAV generation
set_tests_properties(
  wav-gen-g711-8k wav-gen-g711-wrong
  wav-gen-sv56-16k wav-gen-sv56-wrong
  wav-gen-mnru-8k
  wav-gen-g726-8k wav-gen-g726-wrong
  wav-gen-fir-8k
  wav-gen-reverb-16k
  PROPERTIES FIXTURES_SETUP WAV_FILES)

# ==========================================================================
# Case 1: WAV input with MATCHING parameters → should succeed, output matches
# ==========================================================================

# G.711: WAV 8000 Hz input, encode A-law
add_test(NAME wav-g711-match-raw
  COMMAND ${BIN}/g711demo A lilo ${CMAKE_SOURCE_DIR}/src/g711/test_data/sweep.src ${WAV_TEST_DIR}/g711_out_raw.raw 256 1 256)
set_tests_properties(wav-g711-match-raw PROPERTIES
  FIXTURES_REQUIRED WAV_FILES)
add_test(NAME wav-g711-match
  COMMAND ${BIN}/g711demo A lilo ${WAV_TEST_DIR}/sweep_8k.wav ${WAV_TEST_DIR}/g711_out.raw 256 1 256)
set_tests_properties(wav-g711-match PROPERTIES
  FIXTURES_REQUIRED WAV_FILES)
add_test(NAME wav-g711-match-verify
  COMMAND ${CMAKE_COMMAND} -E compare_files ${WAV_TEST_DIR}/g711_out.raw ${WAV_TEST_DIR}/g711_out_raw.raw)
set_tests_properties(wav-g711-match-verify PROPERTIES
  DEPENDS "wav-g711-match;wav-g711-match-raw")

# G.726: WAV 8000 Hz input
# g726demo: removed (g726demo expects A-law/mu-law input, not linear PCM; vbr-g726 WAV tests cover G.726)

# MNRU: WAV 8000 Hz input (pass-through, any rate accepted)
add_test(NAME wav-mnru-match
  COMMAND ${BIN}/mnrudemo -q ${WAV_TEST_DIR}/sine_8k.wav ${WAV_TEST_DIR}/mnru_out.raw 256 1 20 00)
set_tests_properties(wav-mnru-match PROPERTIES
  FIXTURES_REQUIRED WAV_FILES)

# FIR filter: WAV 8000 Hz input (matching default -fs 8000)
add_test(NAME wav-fir-match
  COMMAND ${BIN}/firdemo -q ${WAV_TEST_DIR}/fir_test_8k.wav ${WAV_TEST_DIR}/fir_out.raw 8 0 0 0 0 0)
set_tests_properties(wav-fir-match PROPERTIES
  FIXTURES_REQUIRED WAV_FILES)

# Reverb: WAV 16000 Hz input (pass-through)
add_test(NAME wav-reverb-match
  COMMAND ${BIN}/reverb ${WAV_TEST_DIR}/reverb_input_16k.wav test_data/irtest_le.IR ${WAV_TEST_DIR}/reverb_out.raw
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/reverb)
set_tests_properties(wav-reverb-match PROPERTIES
  FIXTURES_REQUIRED WAV_FILES)

# ==========================================================================
# Case 2: WAV input with WRONG sample rate → should fail (non-zero exit)
# ==========================================================================

# G.711: WAV 48000 Hz input (expects 8000 Hz)
add_test(NAME wav-g711-mismatch
  COMMAND ${BIN}/g711demo A lilo ${WAV_TEST_DIR}/sweep_48k.wav ${WAV_TEST_DIR}/g711_bad.raw 256 1 256)
set_tests_properties(wav-g711-mismatch PROPERTIES
  FIXTURES_REQUIRED WAV_FILES
  WILL_FAIL TRUE)

# SV56: wav_io integration tests disabled (sv56 uses p56-bitdepth raw I/O)

# MNRU: WAV 8000 Hz input with -sf 16000 (explicit mismatch)
add_test(NAME wav-gen-filter-16k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/filter_test_16k.wav 16000 1 16 test_data/test.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/fir)
set_tests_properties(wav-gen-filter-16k PROPERTIES FIXTURES_SETUP WAV_FILES_NOSF)
add_test(NAME wav-filter-no-fs
  COMMAND ${BIN}/filter -q IRS16 ${WAV_TEST_DIR}/filter_test_16k.wav ${WAV_TEST_DIR}/filter_nosf.raw)
set_tests_properties(wav-filter-no-fs PROPERTIES
  FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_NOSF")

# actlevel: WAV 8000 Hz input, no -sf given (default is 16000, should accept WAV rate)
add_test(NAME wav-gen-actlev-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/actlev_8k.wav 8000 1 16 test_data/sweep.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/g711)
set_tests_properties(wav-gen-actlev-8k PROPERTIES FIXTURES_SETUP WAV_FILES_NOSF)
add_test(NAME wav-actlevel-no-sf
  COMMAND ${BIN}/actlev -q ${WAV_TEST_DIR}/actlev_8k.wav)
set_tests_properties(wav-actlevel-no-sf PROPERTIES
  FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_NOSF")

# esdru: WAV 16000 Hz input, no -sf given (default is 48000, should accept WAV rate)
add_test(NAME wav-gen-esdru-16k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/esdru_16k.wav 16000 1 16 test_data/voice.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/sv56)
set_tests_properties(wav-gen-esdru-16k PROPERTIES FIXTURES_SETUP WAV_FILES_NOSF)
add_test(NAME wav-esdru-no-sf
  COMMAND ${BIN}/esdru -e_out ${WAV_TEST_DIR}/esdru_nosf_el.raw 0.2 ${WAV_TEST_DIR}/esdru_16k.wav ${WAV_TEST_DIR}/esdru_nosf.raw)
set_tests_properties(wav-esdru-no-sf PROPERTIES
  FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_NOSF")

# freqresp: WAV 8000 Hz input, no -fs given (default is 16000, should accept WAV rate)
add_test(NAME wav-freqresp-no-fs
  COMMAND ${BIN}/freqresp ${WAV_TEST_DIR}/fir_test_8k.wav ${WAV_TEST_DIR}/fir_test_8k.wav ${WAV_TEST_DIR}/freqresp_nosf.txt)
set_tests_properties(wav-freqresp-no-fs PROPERTIES
  FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# ==========================================================================
# Case 3: WAV output extension → produces valid WAV with correct sample rate
# ==========================================================================

# G.711: WAV input → WAV output (check output has WAV header)
add_test(NAME wav-g711-wav-output
  COMMAND ${BIN}/g711demo A lilo ${WAV_TEST_DIR}/sweep_8k.wav ${WAV_TEST_DIR}/g711_out.wav 256 1 256)
set_tests_properties(wav-g711-wav-output PROPERTIES
  FIXTURES_REQUIRED WAV_FILES)

# Verify the output WAV can be opened and has correct sample rate
add_test(NAME wav-g711-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/g711_out.wav 8000 1 16)
set_tests_properties(wav-g711-wav-output-verify PROPERTIES
  DEPENDS wav-g711-wav-output)

# MNRU: WAV output preserves input rate
add_test(NAME wav-mnru-wav-output
  COMMAND ${BIN}/mnrudemo -q ${WAV_TEST_DIR}/sine_8k.wav ${WAV_TEST_DIR}/mnru_out.wav 256 1 20 00)
set_tests_properties(wav-mnru-wav-output PROPERTIES
  FIXTURES_REQUIRED WAV_FILES)
add_test(NAME wav-mnru-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/mnru_out.wav 8000 1 16)
set_tests_properties(wav-mnru-wav-output-verify PROPERTIES
  DEPENDS wav-mnru-wav-output)

# Reverb: WAV output preserves input rate
add_test(NAME wav-reverb-wav-output
  COMMAND ${BIN}/reverb ${WAV_TEST_DIR}/reverb_input_16k.wav test_data/irtest_le.IR ${WAV_TEST_DIR}/reverb_out.wav
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/reverb)
set_tests_properties(wav-reverb-wav-output PROPERTIES
  FIXTURES_REQUIRED WAV_FILES)
add_test(NAME wav-reverb-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/reverb_out.wav 16000 1 16)
set_tests_properties(wav-reverb-wav-output-verify PROPERTIES
  DEPENDS wav-reverb-wav-output)

# ==========================================================================
# Additional tool coverage — matching WAV tests
# ==========================================================================

# --- Generate additional WAV test files ---
add_test(NAME wav-gen-iir-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/iir_test_8k.wav 8000 1 16 test_data/test.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/iir)
add_test(NAME wav-gen-scal-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/scal_voice_8k.wav 8000 1 16 test_data/voice.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/is54)
add_test(NAME wav-gen-unsup-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/unsup_bigend_8k.wav 8000 1 16 test_data/bigend.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/unsup)
add_test(NAME wav-gen-stereoop-32k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/stereo_L_32k.wav 32000 1 16 test_data/sample.L.32k.1ch.smp
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/stereoop)
add_test(NAME wav-gen-stereoop-R-32k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/stereo_R_32k.wav 32000 1 16 test_data/sample.R.32k.1ch.smp
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/stereoop)
add_test(NAME wav-gen-g711iplc-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/g711iplc_8k.wav 8000 1 16 test_data/f2.le
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/g711iplc)
add_test(NAME wav-gen-vbr-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/vbr_voice_8k.wav 8000 1 16 test_data/voice.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/g726)
add_test(NAME wav-gen-g722-16k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/g722_inp_16k.wav 16000 1 16 test_data/inpsp.bin
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/g722)
add_test(NAME wav-gen-bs1770-48k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/bs1770_sine_48k.wav 48000 1 16 test_data/sine997_0dBFS.pcm
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/bs1770demo)
add_test(NAME wav-gen-esdru-48k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/esdru_stereo_48k.wav 48000 1 16 test_data/stereo_test.pcm
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/esdru)
add_test(NAME wav-gen-filter-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/filter_test_8k.wav 8000 1 16 test_data/test.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/fir)

set_tests_properties(
  wav-gen-iir-8k wav-gen-scal-8k wav-gen-unsup-8k
  wav-gen-stereoop-32k wav-gen-stereoop-R-32k wav-gen-g711iplc-8k
  wav-gen-vbr-8k wav-gen-g722-16k wav-gen-bs1770-48k
  wav-gen-esdru-48k wav-gen-filter-8k
  PROPERTIES FIXTURES_SETUP WAV_FILES_EXT)

# --- IIR filters ---
add_test(NAME wav-cirsdemo-match
  COMMAND ${BIN}/cirsdemo ${WAV_TEST_DIR}/iir_test_8k.wav ${WAV_TEST_DIR}/cirsdemo_out.raw)
set_tests_properties(wav-cirsdemo-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

add_test(NAME wav-c712demo-match
  COMMAND ${BIN}/c712demo ${WAV_TEST_DIR}/iir_test_8k.wav ${WAV_TEST_DIR}/c712demo_out.raw)
set_tests_properties(wav-c712demo-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

add_test(NAME wav-pcmdemo-match
  COMMAND ${BIN}/pcmdemo ${WAV_TEST_DIR}/iir_test_8k.wav ${WAV_TEST_DIR}/pcmdemo_out.raw 1_1 0 0)
set_tests_properties(wav-pcmdemo-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- filter (FIR with -fs) ---
add_test(NAME wav-filter-match
  COMMAND ${BIN}/filter -q IRS8 ${WAV_TEST_DIR}/filter_test_8k.wav ${WAV_TEST_DIR}/filter_out.raw)
set_tests_properties(wav-filter-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- scaldemo ---
add_test(NAME wav-scaldemo-match
  COMMAND ${BIN}/scaldemo -q -trunc ${WAV_TEST_DIR}/scal_voice_8k.wav ${WAV_TEST_DIR}/scal_out.raw 256 1 0 0.5941352)
set_tests_properties(wav-scaldemo-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- stereoop ---
add_test(NAME wav-stereoop-match
  COMMAND ${BIN}/stereoop -q -interleave ${WAV_TEST_DIR}/stereo_L_32k.wav ${WAV_TEST_DIR}/stereo_R_32k.wav ${WAV_TEST_DIR}/stereoop_out.raw)
set_tests_properties(wav-stereoop-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- astrip ---
add_test(NAME wav-astrip-match
  COMMAND ${BIN}/astrip -q -smooth -wlen 145 -sample ${WAV_TEST_DIR}/unsup_bigend_8k.wav ${WAV_TEST_DIR}/astrip_out.raw)
set_tests_properties(wav-astrip-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- fdelay ---
add_test(NAME wav-fdelay-match
  COMMAND ${BIN}/fdelay ${WAV_TEST_DIR}/unsup_bigend_8k.wav ${WAV_TEST_DIR}/fdelay_out.raw)
set_tests_properties(wav-fdelay-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- oper ---
add_test(NAME wav-oper-match
  COMMAND ${BIN}/oper -q 1 ${WAV_TEST_DIR}/unsup_bigend_8k.wav + 1 ${WAV_TEST_DIR}/unsup_bigend_8k.wav 0 ${WAV_TEST_DIR}/oper_out.raw)
set_tests_properties(wav-oper-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- g711iplc ---
add_test(NAME wav-g711iplc-match
  COMMAND ${BIN}/g711iplc -stats ${CMAKE_SOURCE_DIR}/src/g711iplc/test_data/fe10.g192 ${WAV_TEST_DIR}/g711iplc_8k.wav ${WAV_TEST_DIR}/g711iplc_out.raw)
set_tests_properties(wav-g711iplc-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- vbr-g726 ---
add_test(NAME wav-vbr-g726-match
  COMMAND ${BIN}/vbr-g726 -q -law A -rate 16-24-32-40-32-24 ${WAV_TEST_DIR}/vbr_voice_8k.wav ${WAV_TEST_DIR}/vbr_out.raw)
set_tests_properties(wav-vbr-g726-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- g722demo ---
add_test(NAME wav-g722demo-match
  COMMAND ${BIN}/g722demo -q -enc ${WAV_TEST_DIR}/g722_inp_16k.wav ${WAV_TEST_DIR}/g722_out.cod)
set_tests_properties(wav-g722demo-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- encg722 ---
add_test(NAME wav-encg722-match
  COMMAND ${BIN}/encg722 ${WAV_TEST_DIR}/g722_inp_16k.wav ${WAV_TEST_DIR}/encg722_out.cod)
set_tests_properties(wav-encg722-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- bs1770demo ---
add_test(NAME wav-bs1770-match
  COMMAND ${BIN}/bs1770demo ${WAV_TEST_DIR}/bs1770_sine_48k.wav ${WAV_TEST_DIR}/bs1770_out.raw)
set_tests_properties(wav-bs1770-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- esdru ---
add_test(NAME wav-esdru-match
  COMMAND ${BIN}/esdru -e_out ${WAV_TEST_DIR}/esdru_el.raw 0.2 ${WAV_TEST_DIR}/esdru_stereo_48k.wav ${WAV_TEST_DIR}/esdru_out.raw)
set_tests_properties(wav-esdru-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# --- actlevel (read-only measurement) ---
add_test(NAME wav-actlevel-match
  COMMAND ${BIN}/actlev -q ${WAV_TEST_DIR}/voice_16k.wav)
set_tests_properties(wav-actlevel-match PROPERTIES FIXTURES_REQUIRED WAV_FILES)

# --- freqresp (read-only, no output file) ---
add_test(NAME wav-freqresp-match
  COMMAND ${BIN}/freqresp ${WAV_TEST_DIR}/iir_test_8k.wav ${WAV_TEST_DIR}/iir_test_8k.wav ${WAV_TEST_DIR}/freqresp_out.txt)
set_tests_properties(wav-freqresp-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")

# ==========================================================================
# Additional mismatch tests for tools with fixed rates
# ==========================================================================

# vbr-g726: WAV 16000 Hz input (expects 8000 Hz)
add_test(NAME wav-vbr-g726-mismatch
  COMMAND ${BIN}/vbr-g726 -q -law A -rate 16-24-32-40-32-24 ${WAV_TEST_DIR}/g726_voice_16k.wav ${WAV_TEST_DIR}/vbr_bad.raw)
set_tests_properties(wav-vbr-g726-mismatch PROPERTIES
  FIXTURES_REQUIRED WAV_FILES
  WILL_FAIL TRUE)

# g722demo: WAV 8000 Hz input (expects 16000 Hz)
add_test(NAME wav-g722-mismatch
  COMMAND ${BIN}/g722demo ${WAV_TEST_DIR}/sweep_8k.wav ${WAV_TEST_DIR}/g722_bad.raw 256 1 256)
set_tests_properties(wav-g722-mismatch PROPERTIES
  FIXTURES_REQUIRED WAV_FILES
  WILL_FAIL TRUE)

# bs1770demo: WAV 8000 Hz input (expects 48000 Hz)
add_test(NAME wav-bs1770-mismatch
  COMMAND ${BIN}/bs1770demo ${WAV_TEST_DIR}/sweep_8k.wav ${WAV_TEST_DIR}/bs1770_bad.raw)
set_tests_properties(wav-bs1770-mismatch PROPERTIES
  FIXTURES_REQUIRED WAV_FILES
  WILL_FAIL TRUE)

# g711iplc: WAV 16000 Hz input (expects 8000 Hz)
add_test(NAME wav-g711iplc-mismatch
  COMMAND ${BIN}/g711iplc ${CMAKE_SOURCE_DIR}/src/g711iplc/test_data/fe10.g192 ${WAV_TEST_DIR}/g722_inp_16k.wav ${WAV_TEST_DIR}/g711iplc_bad.raw)
set_tests_properties(wav-g711iplc-mismatch PROPERTIES
  FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT"
  WILL_FAIL TRUE)

# ==========================================================================
# More tool coverage
# ==========================================================================

# --- Generate WAV for g727 and rpedemo ---
add_test(NAME wav-gen-g727-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/g727_speech_8k.wav 8000 1 16 test_data/speech.a-s
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/g727)
add_test(NAME wav-gen-sigdiff-8k
  COMMAND ${BIN}/mkwav ${WAV_TEST_DIR}/sigdiff_8k.wav 8000 1 16 test_data/test.src
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/iir)
set_tests_properties(wav-gen-g727-8k wav-gen-sigdiff-8k
  PROPERTIES FIXTURES_SETUP WAV_FILES_EXT2)

# g727demo encode mode
add_test(NAME wav-g727-match
  COMMAND ${BIN}/g727demo -q -core 4 -enh 0 -enc -law A ${WAV_TEST_DIR}/g727_speech_8k.wav ${WAV_TEST_DIR}/g727_out.iad)
set_tests_properties(wav-g727-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT2")

# g727demo: WAV 16000 Hz input (expects 8000)
add_test(NAME wav-g727-mismatch
  COMMAND ${BIN}/g727demo -q -core 4 -enh 0 -enc -law A ${WAV_TEST_DIR}/g722_inp_16k.wav ${WAV_TEST_DIR}/g727_bad.iad)
set_tests_properties(wav-g727-mismatch PROPERTIES
  FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT"
  WILL_FAIL TRUE)

# signal-diff (read-only comparison tool)
add_test(NAME wav-sigdiff-match
  COMMAND ${BIN}/signal-diff -equiv 1 -q sigdiff_8k.wav sigdiff_8k.wav
  WORKING_DIRECTORY ${WAV_TEST_DIR})
set_tests_properties(wav-sigdiff-match PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT;WAV_FILES_EXT2")

# --- decg722 WAV output ---
add_test(NAME wav-decg722-wav-output
  COMMAND ${BIN}/decg722 -q -mode 1 -byte test_data/codspw.cod ${WAV_TEST_DIR}/decg722_out.wav
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/g722)
set_tests_properties(wav-decg722-wav-output PROPERTIES FIXTURES_REQUIRED WAV_FILES)
add_test(NAME wav-decg722-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/decg722_out.wav 16000 1 16)
set_tests_properties(wav-decg722-wav-output-verify PROPERTIES
  DEPENDS wav-decg722-wav-output)

# --- sine WAV output ---
add_test(NAME wav-sine-wav-output
  COMMAND ${BIN}/sine -q ${WAV_TEST_DIR}/sine_out.wav 20000 1000 4 16000)
set_tests_properties(wav-sine-wav-output PROPERTIES FIXTURES_REQUIRED WAV_FILES)
add_test(NAME wav-sine-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/sine_out.wav 16000 1 16)
set_tests_properties(wav-sine-wav-output-verify PROPERTIES DEPENDS wav-sine-wav-output)

# ==========================================================================
# WAV output verification for all remaining tools
# ==========================================================================

# vbr-g726: WAV output at 8000 Hz
add_test(NAME wav-vbr-g726-wav-output
  COMMAND ${BIN}/vbr-g726 -q -law A -rate 16-24-32-40-32-24 ${WAV_TEST_DIR}/vbr_voice_8k.wav ${WAV_TEST_DIR}/vbr_out.wav)
set_tests_properties(wav-vbr-g726-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-vbr-g726-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/vbr_out.wav 8000 1 16)
set_tests_properties(wav-vbr-g726-wav-output-verify PROPERTIES DEPENDS wav-vbr-g726-wav-output)

# g727demo: WAV output at 8000 Hz
add_test(NAME wav-g727-wav-output
  COMMAND ${BIN}/g727demo -q -core 4 -enh 0 -enc -law A ${WAV_TEST_DIR}/g727_speech_8k.wav ${WAV_TEST_DIR}/g727_out.wav)
set_tests_properties(wav-g727-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT2")
add_test(NAME wav-g727-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/g727_out.wav 8000 1 16)
set_tests_properties(wav-g727-wav-output-verify PROPERTIES DEPENDS wav-g727-wav-output)

# g711iplc: WAV output at 8000 Hz
add_test(NAME wav-g711iplc-wav-output
  COMMAND ${BIN}/g711iplc -stats ${CMAKE_SOURCE_DIR}/src/g711iplc/test_data/fe10.g192 ${WAV_TEST_DIR}/g711iplc_8k.wav ${WAV_TEST_DIR}/g711iplc_out.wav)
set_tests_properties(wav-g711iplc-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-g711iplc-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/g711iplc_out.wav 8000 1 16)
set_tests_properties(wav-g711iplc-wav-output-verify PROPERTIES DEPENDS wav-g711iplc-wav-output)

# filter: WAV output at 8000 Hz
add_test(NAME wav-filter-wav-output
  COMMAND ${BIN}/filter -q IRS8 ${WAV_TEST_DIR}/filter_test_8k.wav ${WAV_TEST_DIR}/filter_out.wav)
set_tests_properties(wav-filter-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-filter-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/filter_out.wav 8000 1 16)
set_tests_properties(wav-filter-wav-output-verify PROPERTIES DEPENDS wav-filter-wav-output)

# firdemo: WAV output at 8000 Hz
add_test(NAME wav-fir-wav-output
  COMMAND ${BIN}/firdemo -q ${WAV_TEST_DIR}/fir_test_8k.wav ${WAV_TEST_DIR}/fir_out.wav 8 0 0 0 0 0)
set_tests_properties(wav-fir-wav-output PROPERTIES FIXTURES_REQUIRED WAV_FILES)
add_test(NAME wav-fir-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/fir_out.wav 8000 1 16)
set_tests_properties(wav-fir-wav-output-verify PROPERTIES DEPENDS wav-fir-wav-output)

# cirsdemo: WAV output at 8000 Hz
add_test(NAME wav-cirsdemo-wav-output
  COMMAND ${BIN}/cirsdemo ${WAV_TEST_DIR}/iir_test_8k.wav ${WAV_TEST_DIR}/cirsdemo_out.wav)
set_tests_properties(wav-cirsdemo-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-cirsdemo-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/cirsdemo_out.wav 8000 1 16)
set_tests_properties(wav-cirsdemo-wav-output-verify PROPERTIES DEPENDS wav-cirsdemo-wav-output)

# pcmdemo: WAV output at 8000 Hz
add_test(NAME wav-pcmdemo-wav-output
  COMMAND ${BIN}/pcmdemo ${WAV_TEST_DIR}/iir_test_8k.wav ${WAV_TEST_DIR}/pcmdemo_out.wav 1_1 0 0)
set_tests_properties(wav-pcmdemo-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-pcmdemo-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/pcmdemo_out.wav 8000 1 16)
set_tests_properties(wav-pcmdemo-wav-output-verify PROPERTIES DEPENDS wav-pcmdemo-wav-output)

# c712demo: WAV output at 8000 Hz
add_test(NAME wav-c712demo-wav-output
  COMMAND ${BIN}/c712demo ${WAV_TEST_DIR}/iir_test_8k.wav ${WAV_TEST_DIR}/c712demo_out.wav)
set_tests_properties(wav-c712demo-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-c712demo-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/c712demo_out.wav 8000 1 16)
set_tests_properties(wav-c712demo-wav-output-verify PROPERTIES DEPENDS wav-c712demo-wav-output)

# scaldemo: WAV output at 8000 Hz
add_test(NAME wav-scaldemo-wav-output
  COMMAND ${BIN}/scaldemo -q -trunc ${WAV_TEST_DIR}/scal_voice_8k.wav ${WAV_TEST_DIR}/scal_out.wav 256 1 0 0.5941352)
set_tests_properties(wav-scaldemo-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-scaldemo-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/scal_out.wav 8000 1 16)
set_tests_properties(wav-scaldemo-wav-output-verify PROPERTIES DEPENDS wav-scaldemo-wav-output)

# stereoop: WAV output at 32000 Hz
add_test(NAME wav-stereoop-wav-output
  COMMAND ${BIN}/stereoop -q -interleave ${WAV_TEST_DIR}/stereo_L_32k.wav ${WAV_TEST_DIR}/stereo_R_32k.wav ${WAV_TEST_DIR}/stereoop_out.wav)
set_tests_properties(wav-stereoop-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-stereoop-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/stereoop_out.wav 32000 1 16)
set_tests_properties(wav-stereoop-wav-output-verify PROPERTIES DEPENDS wav-stereoop-wav-output)

# astrip: WAV output at 8000 Hz
add_test(NAME wav-astrip-wav-output
  COMMAND ${BIN}/astrip -q -smooth -wlen 145 -sample ${WAV_TEST_DIR}/unsup_bigend_8k.wav ${WAV_TEST_DIR}/astrip_out.wav)
set_tests_properties(wav-astrip-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-astrip-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/astrip_out.wav 8000 1 16)
set_tests_properties(wav-astrip-wav-output-verify PROPERTIES DEPENDS wav-astrip-wav-output)

# fdelay: WAV output at 8000 Hz
add_test(NAME wav-fdelay-wav-output
  COMMAND ${BIN}/fdelay ${WAV_TEST_DIR}/unsup_bigend_8k.wav ${WAV_TEST_DIR}/fdelay_out.wav)
set_tests_properties(wav-fdelay-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-fdelay-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/fdelay_out.wav 8000 1 16)
set_tests_properties(wav-fdelay-wav-output-verify PROPERTIES DEPENDS wav-fdelay-wav-output)

# oper: WAV output at 8000 Hz
add_test(NAME wav-oper-wav-output
  COMMAND ${BIN}/oper -q 1 ${WAV_TEST_DIR}/unsup_bigend_8k.wav + 1 ${WAV_TEST_DIR}/unsup_bigend_8k.wav 0 ${WAV_TEST_DIR}/oper_out.wav)
set_tests_properties(wav-oper-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-oper-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/oper_out.wav 8000 1 16)
set_tests_properties(wav-oper-wav-output-verify PROPERTIES DEPENDS wav-oper-wav-output)

# esdru: WAV output at 48000 Hz
add_test(NAME wav-esdru-wav-output
  COMMAND ${BIN}/esdru -e_out ${WAV_TEST_DIR}/esdru_el2.raw 0.2 ${WAV_TEST_DIR}/esdru_stereo_48k.wav ${WAV_TEST_DIR}/esdru_out.wav)
set_tests_properties(wav-esdru-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-esdru-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/esdru_out.wav 48000 1 16)
set_tests_properties(wav-esdru-wav-output-verify PROPERTIES DEPENDS wav-esdru-wav-output)

# bs1770demo: WAV output at 48000 Hz
add_test(NAME wav-bs1770-wav-output
  COMMAND ${BIN}/bs1770demo ${WAV_TEST_DIR}/bs1770_sine_48k.wav ${WAV_TEST_DIR}/bs1770_out.wav)
set_tests_properties(wav-bs1770-wav-output PROPERTIES FIXTURES_REQUIRED "WAV_FILES;WAV_FILES_EXT")
add_test(NAME wav-bs1770-wav-output-verify
  COMMAND ${BIN}/test_wav_io_validate ${WAV_TEST_DIR}/bs1770_out.wav 48000 1 16)
set_tests_properties(wav-bs1770-wav-output-verify PROPERTIES DEPENDS wav-bs1770-wav-output)
