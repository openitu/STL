include_directories(../eid)
include_directories(../utl)

add_executable(truncate truncate.c trunc-lib.c ../eid/softbit.c ../utl/ugst-utl.c)
target_link_libraries(truncate m)
