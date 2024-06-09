cmake_minimum_required(VERSION 3.28)
project(exam_mpi)

set(CONAN_EXTERNAL "${CMAKE_CURRENT_SOURCE_DIR}/conan_external")
include(${CONAN_EXTERNAL}/generators/conan_toolchain.cmake)

set(CMAKE_CXX_STANDARD 23)

include_directories(/usr/lib/x86_64-linux-gnu/openmpi/include)
include_directories(/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi)
link_directories(/usr/lib/x86_64-linux-gnu/openmpi/lib)

set(HEADERS
        include/exam_mpi/cli_app.hpp
        include/exam_mpi/helpers.hpp
)

set(SOURCES
        main.cpp
        src/cli_app.cpp
)

add_executable(${PROJECT_NAME}
        ${HEADERS}
        ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
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

find_package(tl-optional REQUIRED)
find_package(CLI11 REQUIRED)
find_package(magic_enum REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        tl::optional
        CLI11::CLI11
        magic_enum::magic_enum
        fmt::fmt
)
