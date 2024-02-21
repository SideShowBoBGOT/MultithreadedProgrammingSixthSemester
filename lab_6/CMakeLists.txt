cmake_minimum_required(VERSION 3.27)
project(lab_6)

set(CMAKE_CXX_STANDARD 20)

include_directories(/usr/lib/x86_64-linux-gnu/openmpi/include)
include_directories(/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi)
link_directories(/usr/lib/x86_64-linux-gnu/openmpi/lib)

add_executable(${PROJECT_NAME} main.cpp
    src/${PROJECT_NAME}/matrix.cpp
    src/${PROJECT_NAME}/matrix_factory.cpp
    src/${PROJECT_NAME}/main_rank.cpp
    src/${PROJECT_NAME}/child_rank.cpp
    src/${PROJECT_NAME}/cli_app.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include/lab_6)

add_subdirectory(external)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        CLI11::CLI11
        magic_enum::magic_enum
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
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