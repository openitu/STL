file(GLOB G728FLOAT_SRC *.c)
add_executable(g728 ${G728FLOAT_SRC})
target_link_libraries(g728 ${M_LIBRARY})

