cmake_minimum_required(VERSION 3.28)
project(filewatch LANGUAGES CXX)

add_library(filewatch INTERFACE)
target_sources(filewatch
    INTERFACE FILE_SET HEADERS BASE_DIRS filewatch FILES
        filewatch/FileWatch.hpp
)
