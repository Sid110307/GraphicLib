cmake_minimum_required(VERSION 3.2)
project(GraphicLib DESCRIPTION "Simple X11 2D graphics library with physics" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    set(CMAKE_CXX_FLAGS "/W4 /WX")
else ()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_executable(graphicLib src/main.cpp)

find_package(X11 REQUIRED)
target_link_libraries(graphicLib X11)
