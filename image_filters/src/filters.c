#include "image_filters/filters.h"

#include "image_filters/internal/filter.h"

#define MEAN_MASK(mask, size) \
double mask[size];            \
for (size_t i = 0; i < size; i++) { \
mask[i] = (double) 1 / (size);\
}

void image_filters_mean3_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_size) {
  MEAN_MASK(mask, 3 * 3);
  filter(pixels, width, height, pixel_size, mask, 5, 5);
}

void image_filters_mean5_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_size) {
  MEAN_MASK(mask, 5 * 5);
  filter(pixels, width, height, pixel_size, mask, 5, 5);
}
