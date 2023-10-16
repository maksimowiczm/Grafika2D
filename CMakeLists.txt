cmake_minimum_required(VERSION 3.26)
project(ImageReader C CXX)

#set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_executable(ImageReader src/main.cpp
        src/draw.cpp
        src/context.cpp)

include_directories(src/include)

add_subdirectory(netpbm)

target_link_libraries(${PROJECT_NAME} netpbm)