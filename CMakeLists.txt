cmake_minimum_required(VERSION 3.26)
project(Project C)

set(CMAKE_C_STANDARD 11)

add_executable(Project src/main.c)

add_subdirectory(shapes)
add_subdirectory(drawio)
add_subdirectory(drawing_app)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(${PROJECT_NAME} PkgConfig::GTK4 shapes drawio drawing_app)
