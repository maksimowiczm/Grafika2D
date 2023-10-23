cmake_minimum_required(VERSION 3.26)
project(ImageReader C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)

add_subdirectory(netpbm)
add_subdirectory(image_adapter)
add_subdirectory(image_filters)

IF (WIN32)
    add_executable(${PROJECT_NAME}
            WIN32
            src/win-main.cpp
    )
    target_link_libraries(${PROJECT_NAME} netpbm image_adapter image_filters)
ELSE (UNIX)
    add_executable(${PROJECT_NAME}
            src/unix-main.cpp
    )
    add_subdirectory(image_gui_gtk)
    target_link_libraries(${PROJECT_NAME} netpbm image_adapter image_gui_gtk image_filters)
ENDIF ()
