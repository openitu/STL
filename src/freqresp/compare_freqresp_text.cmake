# Compare freqresp text output against reference, tolerating CRLF.
# Invoked via: cmake -DGOT=... -DEXPECTED=... -DLABEL=... -P compare_freqresp_text.cmake
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/CompareTextFiles.cmake)
compare_text_files(GOT "${GOT}" EXPECTED "${EXPECTED}" LABEL "${LABEL}")
