include_directories(../utl)

add_executable(p50mnru Bg_mnr70_src.c string_ext.c your_filtering_routines.c)
target_link_libraries(p50mnru ${M_LIBRARY})

#TEST: P50MNRU
