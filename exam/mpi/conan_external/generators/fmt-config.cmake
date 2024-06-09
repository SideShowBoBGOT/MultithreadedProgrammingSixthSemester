########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(fmt_FIND_QUIETLY)
    set(fmt_MESSAGE_MODE VERBOSE)
else()
    set(fmt_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/fmtTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${fmt_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(fmt_VERSION_STRING "10.2.1")
set(fmt_INCLUDE_DIRS ${fmt_INCLUDE_DIRS_DEBUG} )
set(fmt_INCLUDE_DIR ${fmt_INCLUDE_DIRS_DEBUG} )
set(fmt_LIBRARIES ${fmt_LIBRARIES_DEBUG} )
set(fmt_DEFINITIONS ${fmt_DEFINITIONS_DEBUG} )

# Only the first installed configuration is included to avoid the collision
foreach(_BUILD_MODULE ${fmt_BUILD_MODULES_PATHS_DEBUG} )
    message(${fmt_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()

# Check that the specified components in the find_package(Foo COMPONENTS x y z) are there
# This is the variable filled by CMake with the requested components in find_package
if(fmt_FIND_COMPONENTS)
    foreach(_FIND_COMPONENT ${fmt_FIND_COMPONENTS})
        if (TARGET ${_FIND_COMPONENT})
            message(${fmt_MESSAGE_MODE} "Conan: Component '${_FIND_COMPONENT}' found in package 'fmt'")
        else()
            message(FATAL_ERROR "Conan: Component '${_FIND_COMPONENT}' NOT found in package 'fmt'")
        endif()
    endforeach()
endif()

