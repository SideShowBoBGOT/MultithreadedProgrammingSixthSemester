cmake_minimum_required(VERSION 3.27)
project(ParallelBFS)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(External)

add_library(${PROJECT_NAME} INTERFACE
    Include/${PROJECT_NAME}/TAtomicQueue.hpp
    Include/${PROJECT_NAME}/TBaseBFSMixin.hpp
    Include/${PROJECT_NAME}/TCommunicationBFS.hpp
    Include/${PROJECT_NAME}/TDeque.hpp
    Include/${PROJECT_NAME}/THelpers.hpp
    Include/${PROJECT_NAME}/TParallelBFSMixin.hpp
    Include/${PROJECT_NAME}/TPipes.hpp
    Include/${PROJECT_NAME}/TSequentialBFS.hpp
    Include/${PROJECT_NAME}/TSharedBFS.hpp

)
target_include_directories(${PROJECT_NAME} INTERFACE Include)

add_subdirectory(Tests)