add_library(image_filters
        src/internal/filter.c
        src/filters.c
        src/operations.c
)

target_include_directories(image_filters PUBLIC include)
