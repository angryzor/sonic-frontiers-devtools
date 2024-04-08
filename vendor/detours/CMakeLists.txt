cmake_minimum_required(VERSION 3.28)
project(detours LANGUAGES CXX)

add_library(detours STATIC)
target_sources(detours
    PRIVATE
        Detours/src/creatwth.cpp
        Detours/src/disolarm.cpp
        Detours/src/disolx64.cpp
        Detours/src/modules.cpp
        Detours/src/detours.cpp
        Detours/src/disolarm64.cpp
        Detours/src/disolx86.cpp
        Detours/src/disasm.cpp
        Detours/src/disolia64.cpp
        Detours/src/image.cpp
    PUBLIC FILE_SET HEADERS BASE_DIRS Detours/src FILES
        Detours/src/detours.h
    PRIVATE FILE_SET private_headers TYPE HEADERS FILES
        Detours/src/detver.h
        Detours/src/uimports.cpp
)
