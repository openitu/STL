include_directories(../eid)
include_directories(../utl)

#TODO
add_executable(g711iplc g711iplc.c plcferio.c g711iplc.c lowcfe.c ../eid/softbit.c)
target_link_libraries(g711iplc m)

add_executable(asc2g192 asc2g192.c error.c)
target_link_libraries(asc2g192 m)
