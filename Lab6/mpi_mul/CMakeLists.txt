cmake_minimum_required(VERSION 3.27)
project(mpi_mul)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC
        src/child_rank.cpp
        src/mpi_mul/child_rank.hpp
        src/mpi_mul/common.hpp
        src/main_rank.cpp
        src/mpi_mul/main_rank.hpp
        src/matrix.cpp
        src/mpi_mul.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        stdc++
        m
        mpi_cxx
        mpi
        boost_system
        boost_thread
        boost_mpi
        boost_serialization
        pthread
)