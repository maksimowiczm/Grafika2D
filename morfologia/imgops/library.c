#include "include/imgops/library.h"

#define PIXEL_INDEX(x, y, width) ((y) * (width) + (x))

int8_t imgops_hit_or_miss(const uint8_t *image,
                          int64_t image_width,
                          int64_t image_height,
                          int64_t x,
                          int64_t y,
                          const uint8_t *mask,
                          int64_t mask_width,
                          int64_t mask_height) {
  if (x + mask_width > image_width || y + mask_height > image_height) {
    return ERROR;
  }

  if (mask_height != mask_width) {
    return ERROR;
  }

  size_t hits = 0; // count how many hits pixel hit
  size_t mask_times = 0; // count how many trues in mask

  int64_t center = mask_height / 2;
  for (int64_t j = -center; j <= center; j++) {
    if ((y + j) < 0) {
      return MISS;
    }

    for (int64_t i = -center; i <= center; i++) {
      if ((x + i) < 0) {
        return MISS;
      }

      const bool mask_value = mask[PIXEL_INDEX(i + center, j + center, mask_width)];

      // skip if mask value is false
      if (mask_value == 0) {
        continue;
      }

      mask_times++;

      // hit if pixel is black
      const size_t pixel_index = PIXEL_INDEX(x + i, y + j, image_width);
      const uint8_t pixel_value = image[pixel_index];
      if (pixel_value == 0) {
        hits++;
      }
    }
  }

  if (hits == 0) {
    return MISS;
  }

  if (hits == mask_times) {
    return FIT;
  }

  return HIT;
}
