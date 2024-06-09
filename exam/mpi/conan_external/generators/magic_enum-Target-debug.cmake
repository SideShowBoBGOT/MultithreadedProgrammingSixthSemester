# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(magic_enum_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(magic_enum_FRAMEWORKS_FOUND_DEBUG "${magic_enum_FRAMEWORKS_DEBUG}" "${magic_enum_FRAMEWORK_DIRS_DEBUG}")

set(magic_enum_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET magic_enum_DEPS_TARGET)
    add_library(magic_enum_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET magic_enum_DEPS_TARGET
             PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${magic_enum_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${magic_enum_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:>
             APPEND)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### magic_enum_DEPS_TARGET to all of them
conan_package_library_targets("${magic_enum_LIBS_DEBUG}"    # libraries
                              "${magic_enum_LIB_DIRS_DEBUG}" # package_libdir
                              "${magic_enum_BIN_DIRS_DEBUG}" # package_bindir
                              "${magic_enum_LIBRARY_TYPE_DEBUG}"
                              "${magic_enum_IS_HOST_WINDOWS_DEBUG}"
                              magic_enum_DEPS_TARGET
                              magic_enum_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "magic_enum"    # package_name
                              "${magic_enum_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${magic_enum_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Debug ########################################
    set_property(TARGET magic_enum::magic_enum
                 PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Debug>:${magic_enum_OBJECTS_DEBUG}>
                 $<$<CONFIG:Debug>:${magic_enum_LIBRARIES_TARGETS}>
                 APPEND)

    if("${magic_enum_LIBS_DEBUG}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET magic_enum::magic_enum
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     magic_enum_DEPS_TARGET
                     APPEND)
    endif()

    set_property(TARGET magic_enum::magic_enum
                 PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Debug>:${magic_enum_LINKER_FLAGS_DEBUG}> APPEND)
    set_property(TARGET magic_enum::magic_enum
                 PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Debug>:${magic_enum_INCLUDE_DIRS_DEBUG}> APPEND)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET magic_enum::magic_enum
                 PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Debug>:${magic_enum_LIB_DIRS_DEBUG}> APPEND)
    set_property(TARGET magic_enum::magic_enum
                 PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Debug>:${magic_enum_COMPILE_DEFINITIONS_DEBUG}> APPEND)
    set_property(TARGET magic_enum::magic_enum
                 PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Debug>:${magic_enum_COMPILE_OPTIONS_DEBUG}> APPEND)

########## For the modules (FindXXX)
set(magic_enum_LIBRARIES_DEBUG magic_enum::magic_enum)
