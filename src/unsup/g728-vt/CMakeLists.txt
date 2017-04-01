include_directories(../../utl)

add_executable(unsup_snr snr.c)
target_link_libraries(unsup_snr  ${M_LIBRARY})

#TODO Does not compile: undefined reference to `i_dnnt'
#add_executable(wsnr wsnr.c ldcsub.c s_rnge.c sig_die.c)
#target_link_libraries(wsnr m)

#TODO Does not compile: undefined reference to `i_dnnt'
#add_executable(ldcdec ldcdec ldcdec.c ldcsub.c s_rnge.c sig_die.c)
#target_link_libraries(ldcdec m)

add_executable(cwcomp cwcomp.c)
