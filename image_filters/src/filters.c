#include "image_filters/filters.h"

#include "image_filters/internal/filter.h"
#include "string.h"

void image_filters_mean_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_length, size_t size) {
  double *mask = malloc(sizeof(double) * size * size);
  for (size_t i = 0; i < size * size; i++) {
    mask[i] = (double) 1 / (size * size);
  }
  mask_filter(pixels, width, height, pixel_length, mask, size, size);
  free(mask);
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

void image_filters_sobel_filter(uint8_t *pixels, size_t width, size_t height, size_t pixel_length, size_t size) {
  double mask[9] = {-1, 0, 1,
                    -2, 0, 2,
                    -1, 0, 1};
  mask_filter(pixels, width, height, pixel_length, mask, 3, 3);
}
