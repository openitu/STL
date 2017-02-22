include_directories(../utl)

add_executable(reverb reverb.c reverb-lib.c)
target_link_libraries(reverb m)
