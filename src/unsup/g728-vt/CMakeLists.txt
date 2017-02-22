include_directories(../../utl)

#TODO needs math.h:log10 but redifines round...
#add_executable(unsup_snr snr.c)
#target_link_libraries(unsup_snr m)

#TODO needs math.h:log10 but redifines round...
#add_executable(wsnr wsnr.c ldcsub.c s_rnge.c sig_die.c)
#target_link_libraries(wsnr m)

#TODO needs math.h:log10 but redifines round...
#add_executable(ldcdec ldcdec ldcdec.c ldcsub.c s_rnge.c sig_die.c)
#target_link_libraries(ldcdec m)

add_executable(cwcomp cwcomp.c)
