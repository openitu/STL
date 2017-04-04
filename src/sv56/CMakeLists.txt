include_directories(../g711)
include_directories(../utl)

add_executable(sv56demo sv56demo.c  sv-p56.c ../utl/ugst-utl.c)
target_link_libraries(sv56demo ${M_LIBRARY})

add_executable(actlev actlevel.c  sv-p56.c ../utl/ugst-utl.c)
target_link_libraries(actlev ${M_LIBRARY})

