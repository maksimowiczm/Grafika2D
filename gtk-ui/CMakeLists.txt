add_library(
        color_ui
        src/internal/window.c
        src/app.c
)

target_include_directories(color_ui PUBLIC include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(color_ui PUBLIC PkgConfig::GTK4 color_converter)
