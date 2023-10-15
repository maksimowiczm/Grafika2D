cmake_minimum_required(VERSION 3.26)
project(ImageReader C)

set(CMAKE_C_STANDARD 11)

add_executable(ImageReader main.c)

add_subdirectory(netpbm)

target_link_libraries(${PROJECT_NAME} netpbm)