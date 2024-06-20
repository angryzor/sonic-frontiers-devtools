cmake_minimum_required(VERSION 3.28)
# DevTools
project(devtools VERSION 0.1.23
                 DESCRIPTION "Hedgehog Engine 2 DevTools"
                 LANGUAGES CXX)

set(PROJECT_AUTHOR "angryzor")
set(PROJECT_COPYRIGHT "Copyright (c) 2024 angryzor - Licensed under GPL3")
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d")

add_subdirectory(vendor EXCLUDE_FROM_ALL)

# Settings for target sdks
set(SDK_VAR_wars "SONIC_FORCES_SDK")
set(GAME_FOLDER_DEFAULT_wars "C:\\Program Files (x86)\\Steam\\steamapps\\common\\SonicForces")
set(GAME_EXECUTABLE_wars "build/main/projects/exec/Sonic Forces.exe")
set(DEBUGGER_WORKING_DIRECTORY_wars "build/main/projects/exec")
set(MOD_SUBFOLDER_wars "build/main/projects/exec/Mods/devtools")
set(UPDATE_SERVER_NAMESPACE_wars "sonic-forces")
set(FRIENDLY_GAME_NAME_wars "Sonic Forces")

set(SDK_VAR_rangers "SONIC_FRONTIERS_SDK")
set(GAME_FOLDER_DEFAULT_rangers "C:\\Program Files (x86)\\Steam\\steamapps\\common\\SonicFrontiers")
set(GAME_EXECUTABLE_rangers "SonicFrontiers.exe")
set(DEBUGGER_WORKING_DIRECTORY_rangers "")
set(MOD_SUBFOLDER_rangers "Mods/devtools")
set(UPDATE_SERVER_NAMESPACE_rangers "sonic-frontiers")
set(FRIENDLY_GAME_NAME_rangers "Sonic Frontiers")

# The main target
foreach(DEVTOOLS_TARGET_SDK rangers wars)
    set(PROJECT_DESCRIPTION "${FRIENDLY_GAME_NAME_${DEVTOOLS_TARGET_SDK}} DevTools")
    set(PROJECT_LONG_DESCRIPTION "In-engine development kit for ${FRIENDLY_GAME_NAME_${DEVTOOLS_TARGET_SDK}}")
    set(GAME_FOLDER_${DEVTOOLS_TARGET_SDK} ${GAME_FOLDER_DEFAULT_${DEVTOOLS_TARGET_SDK}} CACHE STRING "The location of your ${DEVTOOLS_TARGET_SDK} game folder.")
    set(DEVTOOLS_TARGET devtools-${DEVTOOLS_TARGET_SDK})
    set(UPDATE_SERVER_NAMESPACE ${UPDATE_SERVER_NAMESPACE_${DEVTOOLS_TARGET_SDK}})
    cmake_path(SET GAME_FOLDER_NORMALIZED NORMALIZE ${GAME_FOLDER_${DEVTOOLS_TARGET_SDK}})
    cmake_path(APPEND GAME_FOLDER_NORMALIZED ${GAME_EXECUTABLE_${DEVTOOLS_TARGET_SDK}} OUTPUT_VARIABLE GAME_EXECUTABLE)
    cmake_path(APPEND GAME_FOLDER_NORMALIZED ${DEBUGGER_WORKING_DIRECTORY_${DEVTOOLS_TARGET_SDK}} OUTPUT_VARIABLE DEBUGGER_WORKING_DIRECTORY)
    cmake_path(APPEND GAME_FOLDER_NORMALIZED ${MOD_SUBFOLDER_${DEVTOOLS_TARGET_SDK}} OUTPUT_VARIABLE MOD_FOLDER)

    if(DEFINED ENV{${SDK_VAR_${DEVTOOLS_TARGET_SDK}}})
        message("${SDK_VAR_${DEVTOOLS_TARGET_SDK}} environment variable found. Using this version instead of the local submodule.")
        cmake_path(SET SDK_PATH NORMALIZE $ENV{${SDK_VAR_${DEVTOOLS_TARGET_SDK}}})
    else()
        set(SDK_PATH ${DEVTOOLS_TARGET_SDK}-sdk)
    endif()

    add_subdirectory(${SDK_PATH} ${DEVTOOLS_TARGET_SDK}-sdk EXCLUDE_FROM_ALL)

    add_library(${DEVTOOLS_TARGET} SHARED)
    target_compile_features(${DEVTOOLS_TARGET} PRIVATE cxx_std_17)
    target_compile_options(${DEVTOOLS_TARGET} PRIVATE /ZI /permissive-)
    target_compile_definitions(${DEVTOOLS_TARGET}
        PRIVATE
            DEVTOOLS_BINARY_FILENAME=$<TARGET_FILE_NAME:${DEVTOOLS_TARGET}>
            DEVTOOLS_TARGET_SDK="${DEVTOOLS_TARGET_SDK}"
            DEVTOOLS_TARGET_SDK_${DEVTOOLS_TARGET_SDK}=1
            DEVTOOLS_PROJECT_AUTHOR="${PROJECT_AUTHOR}"
            DEVTOOLS_PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}"
            DEVTOOLS_PROJECT_LONG_DESCRIPTION="${PROJECT_LONG_DESCRIPTION}"
            DEVTOOLS_PROJECT_COPYRIGHT="${PROJECT_COPYRIGHT}"
            DEVTOOLS_BUILD_DATE="${PROJECT_BUILD_DATE}"
            DEVTOOLS_VERSION=${PROJECT_VERSION}
            DEVTOOLS_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
            DEVTOOLS_VERSION_MINOR=${PROJECT_VERSION_MINOR}
            DEVTOOLS_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    )
    set_target_properties(${DEVTOOLS_TARGET}
        PROPERTIES
            VS_DEBUGGER_COMMAND ${GAME_EXECUTABLE}
            VS_DEBUGGER_WORKING_DIRECTORY ${DEBUGGER_WORKING_DIRECTORY}
    )

    foreach(LIB ${DEVTOOLS_TARGET_SDK}-sdk HedgeLib filewatch detours imgui imgui-backend-win32 imgui-backend-dx11 imgui-file-dialog imguizmo implot)
        target_link_libraries(${DEVTOOLS_TARGET} PRIVATE ${LIB})
    endforeach()

    add_subdirectory(src ${DEVTOOLS_TARGET}/src)
    add_subdirectory(natvis ${DEVTOOLS_TARGET}/natvis)
    configure_file(Mod.ini.in ${DEVTOOLS_TARGET}/Mod.ini)

    install(TARGETS ${DEVTOOLS_TARGET} RUNTIME DESTINATION "${MOD_FOLDER}")
    install(DIRECTORY resources/common/ DESTINATION "${MOD_FOLDER}")
    install(DIRECTORY resources/${DEVTOOLS_TARGET_SDK}/ DESTINATION "${MOD_FOLDER}")
    install(FILES LICENSE.txt "${CMAKE_CURRENT_BINARY_DIR}/${DEVTOOLS_TARGET}/Mod.ini" DESTINATION "${MOD_FOLDER}")
endforeach()
