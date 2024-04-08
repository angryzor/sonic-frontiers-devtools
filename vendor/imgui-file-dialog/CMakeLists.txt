cmake_minimum_required(VERSION 3.28)
project(imgui-file-dialog LANGUAGES CXX)

add_library(imgui-file-dialog STATIC)
target_compile_features(imgui-file-dialog PRIVATE cxx_std_17)
target_compile_definitions(imgui-file-dialog PUBLIC USE_STD_FILESYSTEM IMGUIFILEDIALOG_NO_EXPORT)
target_link_libraries(imgui-file-dialog PUBLIC imgui)
target_sources(imgui-file-dialog
    PRIVATE
        ImGuiFileDialog/ImGuiFileDialog.cpp
    PUBLIC FILE_SET HEADERS BASE_DIRS ImGuiFileDialog FILES
        ImGuiFileDialog/ImGuiFileDialog.h
)
