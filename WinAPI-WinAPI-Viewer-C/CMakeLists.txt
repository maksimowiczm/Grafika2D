cmake_minimum_required(VERSION 3.26)
project(ImageReader C)

set(CMAKE_C_STANDARD 11)

add_executable(ImageReader src/main.c
        src/draw.c
        src/context.c)

include_directories(src/include)

target_link_libraries(${PROJECT_NAME} netpbm)