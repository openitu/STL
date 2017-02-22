include_directories(../iir)
include_directories(../utl)


add_executable(filter filter.c fir-dsm.c fir-flat.c fir-irs.c fir-lib.c fir-pso.c fir-tia.c fir-hirs.c fir-wb.c fir-msin.c fir-LP.c ../iir/iir-lib.c ../iir/iir-g712.c ../iir/iir-dir.c ../iir/iir-flat.c ../utl/ugst-utl.c)
target_link_libraries(filter m)

add_executable(flt fltresp.c fir-dsm.c fir-flat.c fir-irs.c fir-lib.c fir-pso.c fir-tia.c fir-hirs.c fir-wb.c fir-msin.c fir-LP.c ../iir/iir-lib.c ../iir/iir-g712.c ../iir/iir-dir.c ../iir/iir-flat.c)
target_link_libraries(flt m)

add_executable(firdemo firdemo.c fir-dsm.c fir-flat.c fir-irs.c fir-lib.c fir-pso.c fir-tia.c fir-hirs.c fir-wb.c fir-msin.c fir-LP.c ../iir/iir-lib.c ../iir/iir-g712.c ../iir/iir-dir.c ../iir/iir-flat.c ../utl/ugst-utl.c)
target_link_libraries(firdemo m)
