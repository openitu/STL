# Run gain_chk on bundled PCM and compare stderr to a reference (portable summary
# line with 2 decimal places). Invoked via: cmake -P gain_chk_ctest.cmake
# Required: -DGAIN_CHK=... -DPCM_DIR=... -DWORKDIR=... -DCASE=self|a|b|c|d

if(NOT DEFINED GAIN_CHK)
    message(FATAL_ERROR "gain_chk_ctest.cmake: -DGAIN_CHK=path/to/gain_chk required")
endif()
if(NOT DEFINED PCM_DIR)
    message(FATAL_ERROR "gain_chk_ctest.cmake: -DPCM_DIR=path/to/test_data required")
endif()
if(NOT DEFINED WORKDIR)
    message(FATAL_ERROR "gain_chk_ctest.cmake: -DWORKDIR=writable scratch directory required")
endif()
if(NOT DEFINED CASE)
    message(FATAL_ERROR "gain_chk_ctest.cmake: -DCASE=self|a|b|c|d required")
endif()

file(MAKE_DIRECTORY "${WORKDIR}")
set(_out "${WORKDIR}/results_${CASE}.txt")
set(_err "${WORKDIR}/stderr_${CASE}.txt")
set(_so "${WORKDIR}/stdout_${CASE}.txt")

set(_ref_i "")
set(_proc_o "")
set(_ref_err "")

set(_input "${PCM_DIR}/input48c.pcm")

if(CASE STREQUAL "self")
    set(_ref_i "${_input}")
    set(_proc_o "${_input}")
    set(_ref_err "${PCM_DIR}/gain_chk_stderr_self.ref")
elseif(CASE STREQUAL "a")
    set(_ref_i "${_input}")
    set(_proc_o "${PCM_DIR}/syn.a.pcm")
    set(_ref_err "${PCM_DIR}/gain_chk_stderr_a.ref")
elseif(CASE STREQUAL "b")
    set(_ref_i "${_input}")
    set(_proc_o "${PCM_DIR}/syn.b.pcm")
    set(_ref_err "${PCM_DIR}/gain_chk_stderr_b.ref")
elseif(CASE STREQUAL "c")
    set(_ref_i "${_input}")
    set(_proc_o "${PCM_DIR}/syn.c.pcm")
    set(_ref_err "${PCM_DIR}/gain_chk_stderr_c.ref")
elseif(CASE STREQUAL "d")
    set(_ref_i "${_input}")
    set(_proc_o "${PCM_DIR}/syn.d.pcm")
    set(_ref_err "${PCM_DIR}/gain_chk_stderr_d.ref")
else()
    message(FATAL_ERROR "gain_chk_ctest.cmake: unknown CASE=${CASE}")
endif()

execute_process(
    COMMAND "${GAIN_CHK}" -i "${_ref_i}" -o "${_proc_o}" -t "${_out}" -r 48000
    OUTPUT_FILE "${_so}"
    ERROR_FILE "${_err}"
    RESULT_VARIABLE _rv
)

if(NOT _rv EQUAL 4)
    file(READ "${_so}" _sot)
    file(READ "${_err}" _ert)
    message(FATAL_ERROR "gain_chk exit code ${_rv}, expected 4 (threshold pass).\nSTDERR:\n${_ert}\nSTDOUT:\n${_sot}")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/CompareTextFiles.cmake)

compare_text_files(GOT "${_err}" EXPECTED "${_ref_err}" LABEL "CASE=${CASE}")
