cmake_minimum_required(VERSION 3.26)
project(ImageReader C CXX)

set(CMAKE_CXX_STANDARD 11)

set(ENV{OPENCV_DIR} "C:\\tools\\opencv\\build")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(src/include
        ${OpenCV_INCLUDE_DIRS}
)

add_executable(ImageReader WIN32
        src/main.cpp 
        src/draw.cpp
        src/context.cpp)

include_directories(src/include)

add_subdirectory(netpbm)

target_link_libraries(${PROJECT_NAME} netpbm ${OpenCV_LIBS})