cmake_minimum_required(VERSION 3.27)
project(ParallelBFS)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(External)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE Source)
target_link_libraries(${PROJECT_NAME} INTERFACE RwLock::RwLock)

add_subdirectory(Tests)