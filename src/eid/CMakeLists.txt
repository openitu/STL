include_directories(../utl)

add_executable(bs-stats bs-stats.c softbit.c)
target_link_libraries(bs-stats m)

add_executable(cvt-head cvt-head.c softbit.c)
target_link_libraries(cvt-head m)

#Example: not compiled by default
#add_executable(eid eid.c softbit.c)
#target_link_libraries(eid m)

add_executable(eid8k eid8k.c eid.c eid_io.c)
target_link_libraries(eid8k m)

add_executable(eiddemo eiddemo.c eid.c)
target_link_libraries(eiddemo m)

add_executable(eid-ev eid-ev.c softbit.c)
target_link_libraries(eid-ev m)

add_executable(eid-int eid-int.c softbit.c)
target_link_libraries(eid-int m)

add_executable(eid-xor eid-xor.c softbit.c)
target_link_libraries(eid-xor m)

add_executable(ep-stats ep-stats.c softbit.c)
target_link_libraries(ep-stats m)

add_executable(gen-patt gen-patt.c eid.c eid_io.c softbit.c)
target_link_libraries(gen-patt m)

add_executable(gen_rate_profile gen_rate_profile.c)
target_link_libraries(gen_rate_profile m)

add_executable(g729e_convert_synch g729e_convert_synch.c softbit.c)
target_link_libraries(g729e_convert_synch m)
