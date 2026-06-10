# Compare a basop test output CSV against its reference, tolerating CRLF.
# Invoked via: cmake -DGOT=... -DEXPECTED=... -DLABEL=... -P compare_basop_csv.cmake
include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/CompareTextFiles.cmake)
compare_text_files(GOT "${GOT}" EXPECTED "${EXPECTED}" LABEL "${LABEL}")
