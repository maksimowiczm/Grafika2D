add_library(
        color_converter
        src/rgb_converter.c
        src/cmyk_converter.c
)

target_include_directories(color_converter PUBLIC include)
