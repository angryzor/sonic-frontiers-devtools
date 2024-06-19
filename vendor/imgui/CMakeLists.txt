cmake_minimum_required(VERSION 3.28)
project(imgui LANGUAGES CXX)

add_library(imgui STATIC)
target_compile_features(imgui PRIVATE cxx_std_17)
target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
target_sources(imgui
    PRIVATE
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
    PUBLIC FILE_SET HEADERS BASE_DIRS imgui FILES
        imgui/imgui.h
    PRIVATE FILE_SET private_headers TYPE HEADERS BASE_DIRS imgui FILES
        imgui/imgui_internal.h
)

foreach(BACKEND allegro5 android dx9 dx10 dx11 dx12 glfw glut opengl2 opengl3 sdl2 sdl3 sdlrenderer2 sdlrenderer3 vulkan wgpu win32)
    string(TOUPPER ${BACKEND} BACKEND_UPPER)
    string(CONCAT IS_BACKEND_ENABLED IMGUI_BUILD_BACKEND_ ${BACKEND_UPPER})
    if(${IS_BACKEND_ENABLED})
        add_library(imgui-backend-${BACKEND} STATIC)
        target_compile_features(imgui-backend-${BACKEND} PRIVATE cxx_std_17)
        target_link_libraries(imgui-backend-${BACKEND} PUBLIC imgui)
        target_sources(imgui-backend-${BACKEND}
            PRIVATE
                imgui/backends/imgui_impl_${BACKEND}.cpp
            PUBLIC FILE_SET HEADERS BASE_DIRS imgui FILES
                imgui/backends/imgui_impl_${BACKEND}.h
        )
    endif()
endforeach()
