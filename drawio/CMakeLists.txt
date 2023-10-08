add_library(
        drawio
        src/draw.c
        src/draw_methods.c
        src/cairo.c
)

target_include_directories(drawio PUBLIC include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(drawio PUBLIC shapes PkgConfig::GTK4)
