include_directories(../utl)

add_executable(stereoop stereoop.c)
target_link_libraries(stereoop ${M_LIBRARY})

