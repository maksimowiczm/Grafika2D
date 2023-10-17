add_library(image_viewer imageViewer.cpp)

IF(WIN32)
    set(ENV{OPENCV_DIR} "C:\\tools\\opencv\\build")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
ENDIF ()

find_package(OpenCV REQUIRED)

target_include_directories(image_viewer PUBLIC include ${OpenCV_INCLUDE_DIRS})

target_link_libraries(image_viewer netpbm ${OpenCV_LIBS})
