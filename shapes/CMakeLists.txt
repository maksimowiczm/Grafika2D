add_library(
        shapes
        src/shapes.c
        src/line.c
        src/rectangle.c
        src/shapeLines.c
        src/point.c
)

target_include_directories(shapes PUBLIC include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(shapes PUBLIC PkgConfig::GTK4 m)