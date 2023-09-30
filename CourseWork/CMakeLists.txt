cmake_minimum_required(VERSION 3.26)
project(CourseWork)

set(CMAKE_CXX_STANDARD 23)

add_executable(CourseWork main.cpp
        TGraph.h
        TSyncQueue.h
        TBFS.h)

find_package(Boost 1.74 COMPONENTS program_options system filesystem serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})

include_directories(external/CLI11/include)
include_directories(external/magic_enum/include)

include(FetchContent)

FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG "v4.0.0"
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)
target_link_libraries(${PROJECT_NAME} PRIVATE Microsoft.GSL::GSL)
