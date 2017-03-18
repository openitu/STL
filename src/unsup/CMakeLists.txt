include_directories(../utl)

add_executable(add sub-add.c)
target_compile_definitions(add PRIVATE ADD_FILES)
add_executable(sub sub-add.c)

add_executable(asc2bin asc2bin.c)
target_link_libraries(asc2bin ${M_LIBRARY})

add_executable(astrip astrip.c)
target_link_libraries(astrip ${M_LIBRARY})

add_executable(bin2asc bin2asc.c)
target_link_libraries(bin2asc ${M_LIBRARY})

add_executable(cf compfile.c)
target_link_libraries(cf ${M_LIBRARY})

add_executable(concat concat.c)

add_executable(df dumpfile.c)
target_link_libraries(df ${M_LIBRARY})

add_executable(endian endian.c)

add_executable(fdelay fdelay.c)
target_link_libraries(fdelay ${M_LIBRARY})

add_executable(measure measure.c)
target_link_libraries(measure ${M_LIBRARY})

add_executable(oper oper.c)
target_link_libraries(oper ${M_LIBRARY})

add_executable(sb sb.c)

add_executable(sine sine.c)
target_link_libraries(sine ${M_LIBRARY})

add_executable(chr2sh chr2sh.c)

add_executable(sh2chr sh2chr.c)

add_executable(crc getcrc32.c)

add_executable(xencode xencode.c)

add_executable(xdecode xdecode.c)
