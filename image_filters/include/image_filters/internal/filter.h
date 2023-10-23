#pragma once

#include "inttypes.h"
#include "stdlib.h"

uint8_t *get_pixel_u8(size_t x,
                      size_t y,
                      uint8_t *pixels,
                      size_t pixels_length,
                      size_t width,
                      size_t pixel_size);

double *get_pixel_dd(size_t x,
                     size_t y,
                     double *pixels,
                     size_t pixels_length,
                     size_t width,
                     size_t pixel_size);

void filter(uint8_t *pixels,
            size_t width,
            size_t height,
            size_t pixel_size,
            double *mask,
            size_t mask_width,
            size_t mask_height);
