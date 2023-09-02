cmake_minimum_required(VERSION 3.20)
project(GraphicLib DESCRIPTION "Simple X11 graphics engine with physics" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    set(CMAKE_CXX_FLAGS "/W4 /WX")
else ()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_executable(graphicLib
        src/main.cpp
        src/application.cpp
        src/window.cpp
        src/physics.cpp
)

if (APPLE)
  include_directories(/opt/X11/include)
  link_directories(/opt/X11/lib)
  target_link_libraries(graphicLib m)
else ()
  find_package(X11 REQUIRED)
  target_link_libraries(graphicLib X11 m)
endif ()

