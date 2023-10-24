#pragma once

#include "inttypes.h"
#include "stdlib.h"

void image_filters_mean3_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_size);

void image_filters_mean5_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_size);

void image_filters_median_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_length, size_t size);
