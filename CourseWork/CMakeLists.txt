cmake_minimum_required(VERSION 3.27)
project(ParallelBFS)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(External)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE Include)

add_subdirectory(Tests)