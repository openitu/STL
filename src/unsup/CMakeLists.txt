include_directories(../utl)

#TODO
#add: 	add.o
#add.o:	sub-add.c
#	$(CC) -c $(CC_OPT) -DADD_FILES -o add.o sub-add.c
#sub: 	sub.o
#  sub.o:	sub-add.c
#  	$(CC) -c $(CC_OPT) -o sub.o sub-add.c

add_executable(asc2bin asc2bin.c)
target_link_libraries(asc2bin m)

add_executable(astrip astrip.c)
target_link_libraries(astrip m)

add_executable(bin2asc bin2asc.c)
target_link_libraries(bin2asc m)

add_executable(cf compfile.c)
target_link_libraries(cf m)

add_executable(concat concat.c)

add_executable(df dumpfile.c)
target_link_libraries(df m)

#add_executable(endian endian.c)
#target_link_libraries(asc2bin m)
#endian: endian.c
#	$(CC) -DRUN -o endian endian.c

add_executable(fdelay fdelay.c)
target_link_libraries(fdelay m)

add_executable(measure measure.c)
target_link_libraries(measure m)

add_executable(oper oper.c)
target_link_libraries(oper m)

add_executable(sb sb.c)

add_executable(sine sine.c)
target_link_libraries(sine m)

add_executable(chr2sh chr2sh.c)

add_executable(sh2chr sh2chr.c)

add_executable(crc getcrc32.c)

add_executable(xencode xencode.c)

add_executable(xdecode xdecode.c)
