cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)

project(image-loader-random)

add_subdirectory(netpbm)
add_subdirectory(image_adapter)
IF (UNIX)
    add_subdirectory(image_gui_gtk)
ENDIF ()
add_subdirectory(image_filters)
add_subdirectory(cxx-gtk-opencv)