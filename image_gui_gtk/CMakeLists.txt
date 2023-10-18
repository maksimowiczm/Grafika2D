add_library(
        image_gui_gtk
        src/app.cpp
        src/ImageWrapper.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
pkg_check_modules(GTKMM gtkmm-4.0)

link_directories(${GTKMM_LIBRARY_DIRS})

target_include_directories(image_gui_gtk PUBLIC include ${GTKMM_INCLUDE_DIRS})

target_link_libraries(image_gui_gtk PUBLIC PkgConfig::GTK4 ${GTKMM_LIBRARIES} netpbm image_viewer)
