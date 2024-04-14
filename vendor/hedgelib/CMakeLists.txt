cmake_minimum_required(VERSION 3.28)
project(HedgeLibWrapper)

set(HEDGELIB_BUILD_HEDGERENDER OFF)
set(HEDGELIB_BUILD_HEDGETOOLS OFF)

if(NOT EXISTS ${PROJECT_SOURCE_DIR}/HedgeLib/Dependencies)
    execute_process(
        COMMAND powershell "-File" "${PROJECT_SOURCE_DIR}/HedgeLib/Get-Dependencies.ps1"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/HedgeLib"
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

add_subdirectory(HedgeLib)
