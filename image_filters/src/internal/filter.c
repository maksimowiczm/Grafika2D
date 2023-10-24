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

void mask_filter(uint8_t *pixels,
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
          const uint8_t *pixel = get_pixel_u8(j - startX + x, i - startY + y, pixels, pixels_length, width, pixel_size);
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

void filter(uint8_t *pixels,
            size_t width,
            size_t height,
            size_t pixel_size,
            size_t filter_width,
            size_t filter_height,
            void (*filter_method)(uint8_t *, uint8_t *, size_t)) {
  const size_t startX = filter_width / 2;
  const size_t startY = filter_height / 2;
  const size_t pixels_length = width * height * pixel_size;
  uint8_t *out_pixels = malloc(pixels_length);
  memcpy(out_pixels, pixels, pixels_length);
  if (out_pixels == NULL) {
    return;
  }

  const size_t filter_size = filter_height * filter_width;
  uint8_t *pixels_to_filter = malloc(filter_size);
  if (pixels_to_filter == NULL) {
    return;
  }

  for (size_t i = startY; i < height - startY; i++) {
    for (size_t j = startX; j < width - startX; j++) {
      for (size_t channel = 0; channel < pixel_size; channel++) {
        memset(pixels_to_filter, 0, filter_size);

        uint8_t *outPixel = get_pixel_u8(j, i, out_pixels, pixels_length, width, pixel_size) + channel;
        for (size_t y = 0; y < filter_width; y++) {
          for (size_t x = 0; x < filter_height; x++) {
            const uint8_t pixel =
                get_pixel_u8(j - startX + x, i - startY + y, pixels, pixels_length, width, pixel_size)[channel];
            uint8_t *to_filter = get_pixel_u8(x, y, pixels_to_filter, filter_size, filter_width, 1);
            *to_filter = pixel;
          }
        }
        filter_method(pixels_to_filter, outPixel, filter_size);
      }
    }
  }
  free(pixels_to_filter);

  memcpy(pixels, out_pixels, pixels_length);
  free(out_pixels);
}
