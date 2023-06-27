cmake_minimum_required(VERSION 3.25)
project(CourseWork)

set(CMAKE_CXX_STANDARD 23)

find_package(fmt REQUIRED)
include_directories(${fmt_INCLUDE_DIRS})

add_executable(CourseWork main.cpp TAStar.h AStar.cpp)
target_link_libraries(CourseWork fmt::fmt)
