#include "netpbm/PixMap3.h"

#include "string.h"

PixMap3Image *netpbm_P3_read_from_string(const char *input, size_t length) {
  uint16_t *pixels = malloc(sizeof(uint16_t) * length);
  memset(pixels, 0, sizeof(uint16_t) * length);
  size_t pixels_pos = 0;

  for (size_t i = 2; i < length; i++) {
    if (input[i] == '#') {
      while (input[i++] != '\n');
    }

    if (input[i] < 48 || input[i] > 57) {
      continue;
    }

    char *end;
    uint16_t num = strtol(input + i, &end, 10);
    i = end - input;
    *(pixels + pixels_pos++) = num;
  }

  PixMap3Image *map = (PixMap3Image *) pixels;

  return map;
}
