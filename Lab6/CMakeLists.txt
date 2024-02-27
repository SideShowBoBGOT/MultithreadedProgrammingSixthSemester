cmake_minimum_required(VERSION 3.27)
project(lab_6)

set(CMAKE_CXX_STANDARD 20)

include_directories(/usr/lib/x86_64-linux-gnu/openmpi/include)
include_directories(/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi)
link_directories(/usr/lib/x86_64-linux-gnu/openmpi/lib)

add_executable(${PROJECT_NAME} main.cpp
        src/cli_app.cpp
        src/matrix.cpp
)
# target_compile_options(${PROJECT_NAME} PUBLIC -O3 -ffast-math -funroll-loops)
target_include_directories(${PROJECT_NAME} PRIVATE include)

add_subdirectory(external)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        CLI11::CLI11
        magic_enum::magic_enum
)

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