include_directories(../utl)

file(GLOB UVSELP_SRC *.c *.h)
add_executable(uvselp ${UVSELP_SRC})
target_link_libraries(uvselp ${M_LIBRARY})

