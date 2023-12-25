enable_testing()
include(CTest)

add_executable(TestOne TestOne.cpp)
target_link_libraries(TestOne PRIVATE ${PROJECT_NAME})
target_link_libraries(TestOne PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(TestOne)