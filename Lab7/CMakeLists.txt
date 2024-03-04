cmake_minimum_required(VERSION 3.27)
project(lab_7)

set(CMAKE_CXX_STANDARD 20)

include_directories(/usr/lib/x86_64-linux-gnu/openmpi/include)
include_directories(/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi)
link_directories(/usr/lib/x86_64-linux-gnu/openmpi/lib)

set(HEADERS
        include/lab_7/cli_app.hpp
        include/lab_7/constants.hpp
        include/lab_7/log_error.hpp
        include/lab_7/alg_type.hpp
        include/lab_7/matrix.hpp
        include/lab_7/common.hpp
        include/lab_7/mat_sizes.hpp
        include/lab_7/main_rank.hpp
        include/lab_7/child_rank.hpp
)

set(SOURCES
        main.cpp
        src/cli_app.cpp
        src/common.cpp
        src/matrix.cpp
        src/main_rank.cpp
        src/child_rank.cpp
)

add_executable(${PROJECT_NAME}
        ${HEADERS}
        ${SOURCES}
)
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