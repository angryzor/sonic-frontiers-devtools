cmake_minimum_required(VERSION 3.28)
project(implot LANGUAGES CXX)


add_library(implot STATIC)
target_compile_features(implot PRIVATE cxx_std_17)
target_link_libraries(implot PUBLIC imgui)
target_sources(implot
    PRIVATE
        implot/implot.cpp
        implot/implot_items.cpp
    PUBLIC FILE_SET HEADERS BASE_DIRS implot FILES
        implot/implot.h
)

if(${DEVTOOLS_ENABLE_IMPLOT_DEMO_WINDOW})
	target_sources(imgui
		PRIVATE
			implot/implot_demo.cpp
	)
endif()
