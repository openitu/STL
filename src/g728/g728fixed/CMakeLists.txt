file(GLOB G728FP_SRC *.c)
add_executable(g728fp ${G728FP_SRC})
target_link_libraries(g728fp ${M_LIBRARY})

