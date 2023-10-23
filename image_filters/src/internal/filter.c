#include "image_filters/internal/filter.h"

#include "string.h"

inline uint8_t *get_pixel_u8(size_t x,
                             size_t y,
                             uint8_t *pixels,
                             size_t pixels_length,
                             size_t width,
                             size_t pixel_size) {
  return width * y * pixel_size + x * pixel_size > pixels_length ?
         NULL :
         &pixels[width * y * pixel_size + x * pixel_size];
}

inline double *get_pixel_dd(size_t x,
                            size_t y,
                            double *pixels,
                            size_t pixels_length,
                            size_t width,
                            size_t pixel_size) {
  return width * y * pixel_size + x * pixel_size > pixels_length ?
         NULL :
         &pixels[width * y * pixel_size + x * pixel_size];
}

void filter(uint8_t *pixels,
            size_t width,
            size_t height,
            size_t pixel_size,
            double *mask,
            size_t mask_width,
            size_t mask_height) {
  const size_t startX = mask_width / 2;
  const size_t startY = mask_width / 2;
  const size_t pixels_length = width * height * pixel_size;
  uint8_t *out_pixels = malloc(pixels_length);
  memcpy(out_pixels, pixels, pixels_length);
  if (out_pixels == NULL) {
    return;
  }

  double *sums = malloc(sizeof(double) * pixel_size);
  if (sums == NULL) {
    return;
  }

  for (size_t i = startY; i < height - startY; i++) {
    for (size_t j = startX; j < width - startX; j++) {
      memset(sums, 0, sizeof(double) * pixel_size);

      for (size_t y = 0; y < mask_height; y++) {
        for (size_t x = 0; x < mask_width; x++) {
          const uint8_t *pixel = get_pixel_u8(j - startX + x, i - startY + y, pixels, pixels_length, width, 3);
          const double mask_value = *get_pixel_dd(x, y, mask, mask_width * mask_height, mask_width, 1);

          for (size_t step = 0; step < pixel_size; step++) {
            sums[step] += pixel[step] * mask_value;
          }
        }
      }

      for (size_t step = 0; step < pixel_size; step++) {
        uint8_t *outPixel = get_pixel_u8(j, i, out_pixels, pixels_length, width, pixel_size);
        outPixel[step] = sums[step];
      }
    }
  }

  free(sums);

  memcpy(pixels, out_pixels, pixels_length);
  free(out_pixels);
}
