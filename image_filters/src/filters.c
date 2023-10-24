#include "image_filters/filters.h"

#include "image_filters/internal/filter.h"
#include "string.h"

#define MEAN_MASK(mask, size) \
double mask[size];            \
for (size_t i = 0; i < size; i++) { \
mask[i] = (double) 1 / (size);\
}

void image_filters_mean3_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_size) {
  MEAN_MASK(mask, 3 * 3);
  mask_filter(pixels, width, height, pixel_size, mask, 5, 5);
}

void image_filters_mean5_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_size) {
  MEAN_MASK(mask, 5 * 5);
  mask_filter(pixels, width, height, pixel_size, mask, 5, 5);
}

static int greater(const void *first, const void *second) {
  return *(uint8_t *) first > *(uint8_t *) second - *(uint8_t *) first < *(uint8_t *) second;
}

inline static void
median_filter(uint8_t *pixels, uint8_t *outPixel, size_t size) {
  uint8_t *to_sort = malloc(size);
  memcpy(to_sort, pixels, size);
  qsort(to_sort, 1, size, greater);
  *outPixel = to_sort[size / 2];
  free(to_sort);
}

void image_filters_median_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_length, size_t size) {
  filter(pixels, width, height, pixel_length, size, 10, median_filter);
}
