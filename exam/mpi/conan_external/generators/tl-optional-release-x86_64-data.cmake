########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND tl-optional_COMPONENT_NAMES tl::optional)
list(REMOVE_DUPLICATES tl-optional_COMPONENT_NAMES)
set(tl-optional_FIND_DEPENDENCY_NAMES "")

########### VARIABLES #######################################################################
#############################################################################################
set(tl-optional_PACKAGE_FOLDER_RELEASE "/home/sideshowbobgot/.conan2/p/tl-op1a360da40b2ab/p")
set(tl-optional_BUILD_MODULES_PATHS_RELEASE )


set(tl-optional_INCLUDE_DIRS_RELEASE "${tl-optional_PACKAGE_FOLDER_RELEASE}/include")
set(tl-optional_RES_DIRS_RELEASE )
set(tl-optional_DEFINITIONS_RELEASE )
set(tl-optional_SHARED_LINK_FLAGS_RELEASE )
set(tl-optional_EXE_LINK_FLAGS_RELEASE )
set(tl-optional_OBJECTS_RELEASE )
set(tl-optional_COMPILE_DEFINITIONS_RELEASE )
set(tl-optional_COMPILE_OPTIONS_C_RELEASE )
set(tl-optional_COMPILE_OPTIONS_CXX_RELEASE )
set(tl-optional_LIB_DIRS_RELEASE )
set(tl-optional_BIN_DIRS_RELEASE )
set(tl-optional_LIBRARY_TYPE_RELEASE UNKNOWN)
set(tl-optional_IS_HOST_WINDOWS_RELEASE 0)
set(tl-optional_LIBS_RELEASE )
set(tl-optional_SYSTEM_LIBS_RELEASE )
set(tl-optional_FRAMEWORK_DIRS_RELEASE )
set(tl-optional_FRAMEWORKS_RELEASE )
set(tl-optional_BUILD_DIRS_RELEASE )
set(tl-optional_NO_SONAME_MODE_RELEASE FALSE)


# COMPOUND VARIABLES
set(tl-optional_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${tl-optional_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${tl-optional_COMPILE_OPTIONS_C_RELEASE}>")
set(tl-optional_LINKER_FLAGS_RELEASE
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${tl-optional_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${tl-optional_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${tl-optional_EXE_LINK_FLAGS_RELEASE}>")


set(tl-optional_COMPONENTS_RELEASE tl::optional)
########### COMPONENT tl::optional VARIABLES ############################################

set(tl-optional_tl_optional_INCLUDE_DIRS_RELEASE "${tl-optional_PACKAGE_FOLDER_RELEASE}/include")
set(tl-optional_tl_optional_LIB_DIRS_RELEASE )
set(tl-optional_tl_optional_BIN_DIRS_RELEASE )
set(tl-optional_tl_optional_LIBRARY_TYPE_RELEASE UNKNOWN)
set(tl-optional_tl_optional_IS_HOST_WINDOWS_RELEASE 0)
set(tl-optional_tl_optional_RES_DIRS_RELEASE )
set(tl-optional_tl_optional_DEFINITIONS_RELEASE )
set(tl-optional_tl_optional_OBJECTS_RELEASE )
set(tl-optional_tl_optional_COMPILE_DEFINITIONS_RELEASE )
set(tl-optional_tl_optional_COMPILE_OPTIONS_C_RELEASE "")
set(tl-optional_tl_optional_COMPILE_OPTIONS_CXX_RELEASE "")
set(tl-optional_tl_optional_LIBS_RELEASE )
set(tl-optional_tl_optional_SYSTEM_LIBS_RELEASE )
set(tl-optional_tl_optional_FRAMEWORK_DIRS_RELEASE )
set(tl-optional_tl_optional_FRAMEWORKS_RELEASE )
set(tl-optional_tl_optional_DEPENDENCIES_RELEASE )
set(tl-optional_tl_optional_SHARED_LINK_FLAGS_RELEASE )
set(tl-optional_tl_optional_EXE_LINK_FLAGS_RELEASE )
set(tl-optional_tl_optional_NO_SONAME_MODE_RELEASE FALSE)

# COMPOUND VARIABLES
set(tl-optional_tl_optional_LINKER_FLAGS_RELEASE
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${tl-optional_tl_optional_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${tl-optional_tl_optional_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${tl-optional_tl_optional_EXE_LINK_FLAGS_RELEASE}>
)
set(tl-optional_tl_optional_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${tl-optional_tl_optional_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${tl-optional_tl_optional_COMPILE_OPTIONS_C_RELEASE}>")