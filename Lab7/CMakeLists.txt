cmake_minimum_required(VERSION 3.27)
project(lab_7)

set(CMAKE_CXX_STANDARD 20)

include_directories(/usr/lib/x86_64-linux-gnu/openmpi/include)
include_directories(/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi)
link_directories(/usr/lib/x86_64-linux-gnu/openmpi/lib)

set(HEADERS
        include/lab_6/cli_app.hpp
        include/lab_6/constants.hpp
        include/lab_6/log_error.hpp
        include/lab_6/alg_type.hpp
        include/lab_6/matrix.hpp
        include/lab_6/common.hpp
        include/lab_6/mat_sizes.hpp
        include/lab_6/main_rank.hpp
        include/lab_6/child_rank.hpp
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