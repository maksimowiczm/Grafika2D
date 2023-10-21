cmake_minimum_required(VERSION 3.27)
project(rgb_cube C)

set(CMAKE_C_STANDARD 11)

add_executable(
        ${PROJECT_NAME}
        src/main.c
        src/window.c
        src/cube.c
        src/cube_state/cube_state.c
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC PkgConfig::GTK4)
