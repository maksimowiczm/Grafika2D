#pragma once

#include "inttypes.h"
#include "stdlib.h"

void image_filters_mean_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_length, size_t size);

void image_filters_median_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_length, size_t size);