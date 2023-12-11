cmake_minimum_required(VERSION 3.26)
project(WinAPI-OpenCV-Viewer-CXX-ImageReader C CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME}
        WIN32
        src/win-main.cpp
)

target_link_libraries(${PROJECT_NAME} netpbm image_adapter)
