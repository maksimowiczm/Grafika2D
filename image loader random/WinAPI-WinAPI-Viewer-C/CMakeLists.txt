cmake_minimum_required(VERSION 3.26)
project(WinAPI-WinAPI-Viewer-C-ImageReader C CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} WIN32
        src/main.c
        src/draw.c
        src/context.c)

include_directories(src/include)

target_link_libraries(${PROJECT_NAME} netpbm)