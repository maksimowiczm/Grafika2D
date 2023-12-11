#include "image_operations/operations.h"

void image_operations_add(uint8_t *pixels,
                          size_t pixels_length,
                          size_t channels_count,
                          size_t channel,
                          int16_t value) {
#define ADD 0
  uint8_t operation = value < 0;

  for (size_t i = 0; i < pixels_length; i += channels_count) {
    const size_t index = i + channel;
    const int new_pixel_value = pixels[index] + value;

    if (operation == ADD && new_pixel_value > 255) {
      pixels[index] = 255;
    } else if (operation != ADD && new_pixel_value < 0) {
      pixels[index] = 0;
    } else {
      pixels[index] = (uint8_t) new_pixel_value;
    }
  }
}

inline void image_operations_brightness(uint8_t *pixels, size_t pixels_length, int16_t value) {
  return image_operations_add(pixels,
                              pixels_length,
                              IMAGE_OPERATIONS_CHANNELS_ALL,
                              IMAGE_OPERATIONS_CHANNEL_ALL,
                              value);
}
