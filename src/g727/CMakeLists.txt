include_directories(../basop)
include_directories(../eid)
include_directories(../g711)
include_directories(../utl)

add_executable(discard discard.c)
target_link_libraries(discard m)

add_executable(g727demo g727demo.c g727.c ../g711/g711.c)
target_link_libraries(g727demo m)
