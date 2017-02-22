include_directories(../g711)
include_directories(../utl)

add_executable(rpedemo rpedemo.c add.c code.c debug.c decode.c long_ter.c lpc.c preproce.c rpe.c gsm_dest.c gsm_deco.c gsm_enco.c gsm_expl.c gsm_impl.c gsm_crea.c gsm_prin.c gsm_opti.c rpeltp.c short_te.c table.c ../g711/g711.c)
target_link_libraries(rpedemo m)

add_executable(test-add add_test.c)
target_link_libraries(test-add m)
