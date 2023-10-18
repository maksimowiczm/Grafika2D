add_library(
        image_gui_gtk
        src/app.c
)

target_include_directories(image_gui_gtk PUBLIC include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(image_gui_gtk PUBLIC PkgConfig::GTK4)
