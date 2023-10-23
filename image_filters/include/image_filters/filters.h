#pragma once

#include "inttypes.h"
#include "stdlib.h"

#define FILTER(name) void image_filters_##name##_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_length)

FILTER(mean3);

FILTER(mean5);
