include_directories(../basop)
include_directories(../eid)
include_directories(../g711)
include_directories(../utl)

add_executable(vbr-g726 vbr-g726.c g726.c ../g711/g711.c)
target_link_libraries(vbr-g726 ${M_LIBRARY})

add_executable(g726demo g726demo.c g726.c)
target_link_libraries(g726demo ${M_LIBRARY})
