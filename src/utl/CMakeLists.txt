add_executable(spdemo spdemo.c ugst-utl.c)
target_link_libraries(spdemo ${M_LIBRARY})

add_executable(scaldemo scaldemo.c ugst-utl.c)
target_link_libraries(scaldemo ${M_LIBRARY})
