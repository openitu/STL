include_directories(../basop)
include_directories(../eid)
include_directories(../utl)

add_executable(g722demo g722demo.c funcg722.c g722.c ../basop/basop32.c ../basop/control.c ../basop/count.c ../basop/enh1632.c ../eid/softbit.c)
target_link_libraries(g722demo ${M_LIBRARY})

add_executable(encg722 encg722.c funcg722.c g722.c ../basop/basop32.c ../basop/control.c ../basop/count.c ../basop/enh1632.c ../eid/softbit.c)
target_link_libraries(encg722 ${M_LIBRARY})

add_executable(decg722 decg722.c funcg722.c g722.c ../basop/basop32.c ../basop/control.c ../basop/count.c ../basop/enh1632.c ../eid/softbit.c)
target_link_libraries(decg722 ${M_LIBRARY})
