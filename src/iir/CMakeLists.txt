include_directories(../utl)

add_executable(pcmdemo pcmdemo.c iir-g712.c iir-lib.c ../utl/ugst-utl.c)
target_link_libraries(pcmdemo m)

add_executable(cirsdemo cirsdemo.c iir-irs.c iir-lib.c ../utl/ugst-utl.c)
target_link_libraries(cirsdemo m)

add_executable(c712demo c712demo.c cascg712.c iir-lib.c ../utl/ugst-utl.c)
target_link_libraries(c712demo m)
