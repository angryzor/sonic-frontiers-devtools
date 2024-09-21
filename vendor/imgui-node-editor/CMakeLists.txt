cmake_minimum_required(VERSION 3.28)
project(imgui-node-editor LANGUAGES CXX)

add_library(imgui-node-editor STATIC)
target_compile_features(imgui-node-editor PRIVATE cxx_std_17)
target_link_libraries(imgui-node-editor PUBLIC imgui)
target_sources(imgui-node-editor
    PRIVATE
        imgui-node-editor/crude_json.cpp
        imgui-node-editor/imgui_canvas.cpp
        imgui-node-editor/imgui_node_editor.cpp
        imgui-node-editor/imgui_node_editor_api.cpp
    PUBLIC FILE_SET HEADERS BASE_DIRS imgui-node-editor FILES
        imgui-node-editor/imgui_node_editor.h
)
