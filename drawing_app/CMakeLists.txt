add_library(
        drawing_app
        src/window.c
        src/main_container.c
        src/drawable_area.c
        src/window_state.c
        src/side_menu.c
        src/context.c
)

target_include_directories(drawing_app PUBLIC include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(drawing_app PUBLIC PkgConfig::GTK4 drawio)
