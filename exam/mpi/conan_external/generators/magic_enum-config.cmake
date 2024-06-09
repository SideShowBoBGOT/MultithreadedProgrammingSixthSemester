########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(magic_enum_FIND_QUIETLY)
    set(magic_enum_MESSAGE_MODE VERBOSE)
else()
    set(magic_enum_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/magic_enumTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${magic_enum_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(magic_enum_VERSION_STRING "0.9.5")
set(magic_enum_INCLUDE_DIRS ${magic_enum_INCLUDE_DIRS_DEBUG} )
set(magic_enum_INCLUDE_DIR ${magic_enum_INCLUDE_DIRS_DEBUG} )
set(magic_enum_LIBRARIES ${magic_enum_LIBRARIES_DEBUG} )
set(magic_enum_DEFINITIONS ${magic_enum_DEFINITIONS_DEBUG} )

# Only the first installed configuration is included to avoid the collision
foreach(_BUILD_MODULE ${magic_enum_BUILD_MODULES_PATHS_DEBUG} )
    message(${magic_enum_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()

# Check that the specified components in the find_package(Foo COMPONENTS x y z) are there
# This is the variable filled by CMake with the requested components in find_package
if(magic_enum_FIND_COMPONENTS)
    foreach(_FIND_COMPONENT ${magic_enum_FIND_COMPONENTS})
        if (TARGET ${_FIND_COMPONENT})
            message(${magic_enum_MESSAGE_MODE} "Conan: Component '${_FIND_COMPONENT}' found in package 'magic_enum'")
        else()
            message(FATAL_ERROR "Conan: Component '${_FIND_COMPONENT}' NOT found in package 'magic_enum'")
        endif()
    endforeach()
endif()

