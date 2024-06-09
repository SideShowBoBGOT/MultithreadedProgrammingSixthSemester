########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(cli11_COMPONENT_NAMES "")
set(cli11_FIND_DEPENDENCY_NAMES "")

########### VARIABLES #######################################################################
#############################################################################################
set(cli11_PACKAGE_FOLDER_DEBUG "/home/sideshowbobgot/.conan2/p/cli114963e6c3642a9/p")
set(cli11_BUILD_MODULES_PATHS_DEBUG )


set(cli11_INCLUDE_DIRS_DEBUG "${cli11_PACKAGE_FOLDER_DEBUG}/include")
set(cli11_RES_DIRS_DEBUG )
set(cli11_DEFINITIONS_DEBUG )
set(cli11_SHARED_LINK_FLAGS_DEBUG )
set(cli11_EXE_LINK_FLAGS_DEBUG )
set(cli11_OBJECTS_DEBUG )
set(cli11_COMPILE_DEFINITIONS_DEBUG )
set(cli11_COMPILE_OPTIONS_C_DEBUG )
set(cli11_COMPILE_OPTIONS_CXX_DEBUG )
set(cli11_LIB_DIRS_DEBUG )
set(cli11_BIN_DIRS_DEBUG )
set(cli11_LIBRARY_TYPE_DEBUG UNKNOWN)
set(cli11_IS_HOST_WINDOWS_DEBUG 0)
set(cli11_LIBS_DEBUG )
set(cli11_SYSTEM_LIBS_DEBUG )
set(cli11_FRAMEWORK_DIRS_DEBUG )
set(cli11_FRAMEWORKS_DEBUG )
set(cli11_BUILD_DIRS_DEBUG )
set(cli11_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(cli11_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${cli11_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${cli11_COMPILE_OPTIONS_C_DEBUG}>")
set(cli11_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${cli11_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${cli11_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${cli11_EXE_LINK_FLAGS_DEBUG}>")


set(cli11_COMPONENTS_DEBUG )