cmake_minimum_required(VERSION 3.26)
project(ImageReader C CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(ImageReader
        WIN32
        src/win-main.cpp
)

add_subdirectory(netpbm)
add_subdirectory(image_viewer_openCV)

target_link_libraries(${PROJECT_NAME} netpbm image_viewer)
