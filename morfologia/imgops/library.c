#include "include/imgops/library.h"

#define PIXEL_INDEX(x, y, width) ((y) * width + x)

int8_t imgops_hit_or_miss(const uint8_t *image,
                          uint64_t image_width,
                          uint64_t image_height,
                          uint64_t x,
                          uint64_t y,
                          const uint8_t *mask,
                          uint64_t mask_width,
                          uint64_t mask_height) {
  if (x + mask_width > image_width || y + mask_height > image_height) {
    return ERROR;
  }

  size_t hits = 0; // count how many hits pixel hit
  size_t mask_times = 0; // count how many trues in mask
  for (size_t j = 0; j < mask_height; j++) {
    for (size_t i = 0; i < mask_width; i++) {
      const bool mask_value = mask[PIXEL_INDEX(i, j, mask_width)];

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
