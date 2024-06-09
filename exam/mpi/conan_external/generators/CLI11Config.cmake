########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(CLI11_FIND_QUIETLY)
    set(CLI11_MESSAGE_MODE VERBOSE)
else()
    set(CLI11_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CLI11Targets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${cli11_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(CLI11_VERSION_STRING "2.4.2")
set(CLI11_INCLUDE_DIRS ${cli11_INCLUDE_DIRS_DEBUG} )
set(CLI11_INCLUDE_DIR ${cli11_INCLUDE_DIRS_DEBUG} )
set(CLI11_LIBRARIES ${cli11_LIBRARIES_DEBUG} )
set(CLI11_DEFINITIONS ${cli11_DEFINITIONS_DEBUG} )

# Only the first installed configuration is included to avoid the collision
foreach(_BUILD_MODULE ${cli11_BUILD_MODULES_PATHS_DEBUG} )
    message(${CLI11_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()

# Check that the specified components in the find_package(Foo COMPONENTS x y z) are there
# This is the variable filled by CMake with the requested components in find_package
if(CLI11_FIND_COMPONENTS)
    foreach(_FIND_COMPONENT ${CLI11_FIND_COMPONENTS})
        if (TARGET ${_FIND_COMPONENT})
            message(${CLI11_MESSAGE_MODE} "Conan: Component '${_FIND_COMPONENT}' found in package 'cli11'")
        else()
            message(FATAL_ERROR "Conan: Component '${_FIND_COMPONENT}' NOT found in package 'cli11'")
        endif()
    endforeach()
endif()

