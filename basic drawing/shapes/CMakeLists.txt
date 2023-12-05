add_library(
        shapes
        src/shapes.c
        src/line.c
        src/quadrilateral.c
        src/shapeLines.c
        src/point.c
        src/circle.c
        src/rectangle.c
)

target_include_directories(shapes PUBLIC include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(shapes PUBLIC PkgConfig::GTK4 m)