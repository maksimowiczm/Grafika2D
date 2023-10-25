#pragma once

#include "inttypes.h"
#include "stdlib.h"

#define FILTER_ARGS uint8_t *pixels, size_t width, size_t height, size_t pixel_length, size_t size

void image_filters_mean_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_length, size_t size);

void image_filters_median_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_length, size_t size);

void image_filters_sobel_filter(FILTER_ARGS);

void image_filters_highpass_filter(FILTER_ARGS);
