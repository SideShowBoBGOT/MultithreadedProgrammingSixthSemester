# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(tl-optional_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(tl-optional_FRAMEWORKS_FOUND_DEBUG "${tl-optional_FRAMEWORKS_DEBUG}" "${tl-optional_FRAMEWORK_DIRS_DEBUG}")

set(tl-optional_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET tl-optional_DEPS_TARGET)
    add_library(tl-optional_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET tl-optional_DEPS_TARGET
             PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${tl-optional_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${tl-optional_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:>
             APPEND)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### tl-optional_DEPS_TARGET to all of them
conan_package_library_targets("${tl-optional_LIBS_DEBUG}"    # libraries
                              "${tl-optional_LIB_DIRS_DEBUG}" # package_libdir
                              "${tl-optional_BIN_DIRS_DEBUG}" # package_bindir
                              "${tl-optional_LIBRARY_TYPE_DEBUG}"
                              "${tl-optional_IS_HOST_WINDOWS_DEBUG}"
                              tl-optional_DEPS_TARGET
                              tl-optional_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "tl-optional"    # package_name
                              "${tl-optional_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${tl-optional_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Debug ########################################

    ########## COMPONENT tl::optional #############

        set(tl-optional_tl_optional_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(tl-optional_tl_optional_FRAMEWORKS_FOUND_DEBUG "${tl-optional_tl_optional_FRAMEWORKS_DEBUG}" "${tl-optional_tl_optional_FRAMEWORK_DIRS_DEBUG}")

        set(tl-optional_tl_optional_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET tl-optional_tl_optional_DEPS_TARGET)
            add_library(tl-optional_tl_optional_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET tl-optional_tl_optional_DEPS_TARGET
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${tl-optional_tl_optional_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${tl-optional_tl_optional_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${tl-optional_tl_optional_DEPENDENCIES_DEBUG}>
                     APPEND)

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'tl-optional_tl_optional_DEPS_TARGET' to all of them
        conan_package_library_targets("${tl-optional_tl_optional_LIBS_DEBUG}"
                              "${tl-optional_tl_optional_LIB_DIRS_DEBUG}"
                              "${tl-optional_tl_optional_BIN_DIRS_DEBUG}" # package_bindir
                              "${tl-optional_tl_optional_LIBRARY_TYPE_DEBUG}"
                              "${tl-optional_tl_optional_IS_HOST_WINDOWS_DEBUG}"
                              tl-optional_tl_optional_DEPS_TARGET
                              tl-optional_tl_optional_LIBRARIES_TARGETS
                              "_DEBUG"
                              "tl-optional_tl_optional"
                              "${tl-optional_tl_optional_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET tl::optional
                     PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${tl-optional_tl_optional_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${tl-optional_tl_optional_LIBRARIES_TARGETS}>
                     APPEND)

        if("${tl-optional_tl_optional_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET tl::optional
                         PROPERTY INTERFACE_LINK_LIBRARIES
                         tl-optional_tl_optional_DEPS_TARGET
                         APPEND)
        endif()

        set_property(TARGET tl::optional PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${tl-optional_tl_optional_LINKER_FLAGS_DEBUG}> APPEND)
        set_property(TARGET tl::optional PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${tl-optional_tl_optional_INCLUDE_DIRS_DEBUG}> APPEND)
        set_property(TARGET tl::optional PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${tl-optional_tl_optional_LIB_DIRS_DEBUG}> APPEND)
        set_property(TARGET tl::optional PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${tl-optional_tl_optional_COMPILE_DEFINITIONS_DEBUG}> APPEND)
        set_property(TARGET tl::optional PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${tl-optional_tl_optional_COMPILE_OPTIONS_DEBUG}> APPEND)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET tl::optional PROPERTY INTERFACE_LINK_LIBRARIES tl::optional APPEND)

########## For the modules (FindXXX)
set(tl-optional_LIBRARIES_DEBUG tl::optional)
