cmake_minimum_required(VERSION 3.25)
project(Lab6)

set(CMAKE_CXX_STANDARD 20)
find_package(MPI REQUIRED)
#find_package( Boost 1.40 COMPONENTS program_options REQUIRED )
#include_directories( ${Boost_INCLUDE_DIR} )

add_executable(Lab6 main.cpp LabStuff/MatrixFactory.h LabStuff/MatrixFactory.cpp LabStuff/Matrix.h LabStuff/Matrix.cpp)
target_link_libraries(Lab6 PRIVATE MPI::MPI_CXX)
#target_link_libraries(Lab6 LINK_PUBLIC ${Boost_LIBRARIES})
