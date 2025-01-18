cmake_minimum_required(VERSION 3.28)
project(imtimeline LANGUAGES CXX)


add_library(imtimeline STATIC)
target_compile_features(imtimeline PRIVATE cxx_std_17)
target_link_libraries(imtimeline PUBLIC imgui)
target_sources(imtimeline
    PRIVATE
        imtimeline/imtimeline.cpp
    PUBLIC FILE_SET HEADERS BASE_DIRS imtimeline FILES
        imtimeline/imtimeline.h
)
