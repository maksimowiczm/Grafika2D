add_library(image_adapter imageViewer.cpp
        imageLoader.cpp)

IF(WIN32)
    set(ENV{OPENCV_DIR} "C:\\tools\\opencv\\build")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
ENDIF ()

find_package(OpenCV REQUIRED)

target_include_directories(image_adapter PUBLIC include ${OpenCV_INCLUDE_DIRS})

target_link_libraries(image_adapter netpbm ${OpenCV_LIBS})
