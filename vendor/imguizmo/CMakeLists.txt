cmake_minimum_required(VERSION 3.28)
project(imguizmo LANGUAGES CXX)

add_library(imguizmo STATIC)
target_compile_features(imguizmo PRIVATE cxx_std_17)
target_link_libraries(imguizmo PUBLIC imgui)
target_sources(imguizmo
    PRIVATE
        ImGuizmo/ImGuizmo.cpp
    PUBLIC FILE_SET HEADERS BASE_DIRS ImGuizmo FILES
        ImGuizmo/ImGuizmo.h
)
