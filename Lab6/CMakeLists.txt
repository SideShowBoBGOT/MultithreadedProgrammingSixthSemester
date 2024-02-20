cmake_minimum_required(VERSION 3.27)
project(lab_6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/usr/bin/mpiCC")

add_executable(${PROJECT_NAME} main.cpp
    src/${PROJECT_NAME}/matrix.cpp
    src/${PROJECT_NAME}/matrix_factory.cpp
    src/${PROJECT_NAME}/main_rank.cpp
    src/${PROJECT_NAME}/child_rank.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include/lab_6)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        stdc++
        m
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        boost_system
        boost_thread
        boost_mpi
        boost_serialization
        boost_log
        boost_log_setup
        pthread
)
