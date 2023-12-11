cmake_minimum_required(VERSION 3.26)
project(cxx-gtk-opencv-ImageReader C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)

IF (WIN32)
    add_executable(${PROJECT_NAME}
            WIN32
            src/win-main.cpp
    )
    target_link_libraries(${PROJECT_NAME} netpbm image_adapter image_filters)
ELSE (UNIX)
    add_executable(${PROJECT_NAME}
            src/unix-main.cpp
    )
    target_link_libraries(${PROJECT_NAME} netpbm image_adapter image_gui_gtk image_filters)
ENDIF ()
