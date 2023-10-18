cmake_minimum_required(VERSION 3.26)
project(ImageReader C CXX)

set(CMAKE_CXX_STANDARD 11)

IF (WIN32)
    add_executable(${PROJECT_NAME}
            WIN32
            src/win-main.cpp
    )
ELSE (UNIX)
    add_executable(${PROJECT_NAME}
            src/unix-main.cpp
    )
ENDIF ()

add_subdirectory(netpbm)
add_subdirectory(image_viewer_openCV)
add_subdirectory(image_gui_gtk)

target_link_libraries(${PROJECT_NAME} netpbm image_viewer image_gui_gtk)
