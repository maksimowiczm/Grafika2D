add_library(
        color_ui
        src/app.c
        src/internal/window.c
        src/internal/rgb_container.c
        src/internal/uint8_picker.c
        src/internal/cmyk_container.c
        src/internal/colors_context.c
        src/internal/color_container.c
)

target_include_directories(color_ui PUBLIC include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(color_ui PUBLIC PkgConfig::GTK4 color_converter shared)
