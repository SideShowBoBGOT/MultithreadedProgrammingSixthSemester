enable_testing()
include(CTest)

add_executable(Benchmark Benchmark.cpp)
target_link_libraries(Benchmark PRIVATE ${PROJECT_NAME})
target_link_libraries(Benchmark PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Benchmark)