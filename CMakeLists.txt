cmake_minimum_required(VERSION 3.27)
project(rgb2cmyk C)

set(CMAKE_C_STANDARD 11)

add_executable(rgb2cmyk main.c)
add_subdirectory(color_converter)

IF (UNIX)
    add_subdirectory(gtk-ui)
ENDIF ()

target_link_libraries(${PROJECT_NAME} color_converter color_ui)
