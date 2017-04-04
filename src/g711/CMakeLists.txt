include_directories(../utl)

add_executable(g711demo g711demo.c g711.c)
target_link_libraries(g711demo ${M_LIBRARY})

add_executable(shiftbit shiftbit.c)
