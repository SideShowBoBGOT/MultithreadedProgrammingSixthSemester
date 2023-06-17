cmake_minimum_required(VERSION 3.25)
project(Lab6)

set(CMAKE_CXX_STANDARD 20)
find_package(MPI REQUIRED)

add_executable(Lab6 main.cpp BlockingMPI.cpp)
target_link_libraries(Lab6 PRIVATE MPI::MPI_CXX)
