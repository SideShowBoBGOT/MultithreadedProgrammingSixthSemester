enable_testing()
include(CTest)

add_executable(Benchmark Benchmark.cpp)
target_link_libraries(Benchmark PRIVATE ${PROJECT_NAME})
target_link_libraries(Benchmark PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Benchmark)

add_executable(Pipes Pipes.cpp)
target_link_libraries(Pipes PRIVATE ${PROJECT_NAME})
target_link_libraries(Pipes PRIVATE GTest::gtest_main)

add_executable(Deque Deque.cpp)
target_link_libraries(Deque PRIVATE ${PROJECT_NAME})
target_link_libraries(Deque PRIVATE GTest::gtest_main)
