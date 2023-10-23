add_library(image_filters
        src/internal/filter.c
        src/filters.c
)

target_include_directories(image_filters PUBLIC include)
