cmake_minimum_required(VERSION 3.28)
# DevTools
project(devtools VERSION 0.1.20
                 DESCRIPTION "Sonic Forces DevTools"
                 LANGUAGES CXX)

set(PROJECT_AUTHOR "angryzor")
set(PROJECT_LONG_DESCRIPTION "In-engine development kit for Sonic Forces")
set(PROJECT_COPYRIGHT "Copyright (c) 2024 angryzor - Licensed under GPL3")
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d")

set(DEVTOOLS_GAME_FOLDER "C:\\Program Files (x86)\\Steam\\steamapps\\common\\SonicForces" CACHE STRING "The location of your game folder.")
cmake_path(SET DEVTOOLS_GAME_FOLDER_NORMALIZED NORMALIZE ${DEVTOOLS_GAME_FOLDER})
cmake_path(APPEND DEVTOOLS_GAME_FOLDER_NORMALIZED "build/main/projects/exec/Sonic Forces.exe" OUTPUT_VARIABLE SONIC_FORCES_GAME_EXECUTABLE)

if(DEFINED ENV{SONIC_FORCES_SDK})
    message("SONIC_FORCES_SDK environment variable found. Using this version instead of the local submodule.")
    cmake_path(SET WARS_SDK_PATH NORMALIZE $ENV{SONIC_FORCES_SDK})
else()
    set(WARS_SDK_PATH wars-sdk)
endif()

add_subdirectory(vendor EXCLUDE_FROM_ALL)
add_subdirectory(${WARS_SDK_PATH} wars-sdk EXCLUDE_FROM_ALL)

# DevTools
add_library(devtools SHARED)
target_compile_features(devtools PRIVATE cxx_std_17)
target_compile_options(devtools PRIVATE /ZI /permissive-)
target_compile_definitions(devtools
    PRIVATE
        DEVTOOLS_BINARY_FILENAME=$<TARGET_FILE_NAME:devtools>
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
set_target_properties(devtools
    PROPERTIES
        VS_DEBUGGER_COMMAND ${SONIC_FORCES_GAME_EXECUTABLE}
        VS_DEBUGGER_WORKING_DIRECTORY "${DEVTOOLS_GAME_FOLDER_NORMALIZED}/build/main/projects/exec"
)

foreach(LIB wars-sdk HedgeLib filewatch detours imgui imgui-backend-win32 imgui-backend-dx11 imgui-file-dialog imguizmo implot)
    target_link_libraries(devtools PRIVATE ${LIB})
endforeach()

add_subdirectory(src)
add_subdirectory(natvis)
configure_file(Mod.ini.in Mod.ini)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT devtools)

cmake_path(APPEND DEVTOOLS_GAME_FOLDER_NORMALIZED "build/main/projects/exec/Mods/devtools" OUTPUT_VARIABLE MOD_FOLDER)

install(TARGETS devtools RUNTIME DESTINATION "${MOD_FOLDER}")
install(DIRECTORY resources/ DESTINATION "${MOD_FOLDER}")
install(FILES LICENSE.txt "${CMAKE_CURRENT_BINARY_DIR}/Mod.ini" DESTINATION "${MOD_FOLDER}")
