# cmake/CompareTextFiles.cmake
# Compare two text files, ignoring line-ending differences (CRLF vs LF).
#
# Usage (from a -P script):
#   include(/path/to/CompareTextFiles.cmake)
#   compare_text_files(GOT <file> EXPECTED <file> [LABEL <name>])

function(compare_text_files)
    cmake_parse_arguments(_CTF "" "GOT;EXPECTED;LABEL" "" ${ARGN})

    file(READ "${_CTF_GOT}" _got)
    string(REPLACE "\r\n" "\n" _got "${_got}")

    file(READ "${_CTF_EXPECTED}" _want)
    string(REPLACE "\r\n" "\n" _want "${_want}")

    if(NOT _got STREQUAL _want)
        message(FATAL_ERROR "Text file mismatch: ${_CTF_LABEL}\n--- got ---\n${_got}\n--- expected ---\n${_want}")
    endif()
endfunction()
