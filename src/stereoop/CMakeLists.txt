include_directories(../utl)

add_executable(stereoop stereoop.c)
target_link_libraries(stereoop ${M_LIBRARY})

add_test(stereoop1-interleave ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/stereoop -q -interleave test_data/sample.L.32k.1ch.smp test_data/sample.R.32k.1ch.smp test_data/sample.LR.32k.2ch.smp )
add_test(stereoop1-split ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/stereoop -q -split test_data/sample.LR.32k.2ch.smp  test_data/sample.L.split.32k.1ch.smp test_data/sample.R.split.32k.1ch.smp)
add_test(stereoop1-left-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sample.L.32k.1ch.smp test_data/sample.L.split.32k.1ch.smp)
add_test(stereoop1-right-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sample.R.32k.1ch.smp test_data/sample.R.split.32k.1ch.smp)

add_test(stereoop2-left ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/stereoop -q -left test_data/sample.LR.32k.2ch.smp  test_data/sample.L.left.32k.1ch.smp)
add_test(stereoop2-leftverify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sample.L.32k.1ch.smp test_data/sample.L.left.32k.1ch.smp)
add_test(stereoop2-right ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/stereoop -q -right test_data/sample.LR.32k.2ch.smp  test_data/sample.R.right.32k.1ch.smp)
add_test(stereoop2-right-verify ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cf -q test_data/sample.R.32k.1ch.smp test_data/sample.R.right.32k.1ch.smp)

add_test(stereoop3 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/stereoop -q -maxenval test_data/sample.LR.32k.2ch.smp  test_data/sample.maxenval.32k.1ch.smp)

add_test(stereoop4 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/stereoop -q -mono test_data/sample.LR.32k.2ch.smp  test_data/sample.mono.32k.1ch.smp)
