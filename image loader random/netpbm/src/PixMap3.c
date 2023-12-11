#include "netpbm/PixMap3.h"

#include "string.h"
#include "ctype.h"
#include "stdlib.h"

PixMap3Image *netpbm_P3_read_from_string(const char *input, size_t length) {
  size_t headerEnd = 0;
  PixMapHeader header;
  netpbm_pix_map_read_header(&header, &headerEnd, input, length);
  if (headerEnd == 0) {
    return NULL;
  }

  size_t pixels_count = header.height * header.width * 3;
  void *pixels = malloc(sizeof(PixMapHeader) + sizeof(uint8_t) * pixels_count);
  memset(pixels, 0, sizeof(PixMapHeader) + sizeof(uint8_t) * pixels_count);

  size_t pixels_pos = sizeof(PixMapHeader);
  for (size_t i = headerEnd; i < length; i++) {

    // skip comments
    if (input[i] == '#') {
      while (input[i] != '\n' && input[i++] != '\r');
    }

    // continue if whitespace
    if (isspace(input[i])) {
      continue;
    }

    // throw if not digit
    if (!isdigit(input[i])) {
      free(pixels);
      return NULL;
    }

    // read number
    char *end;
    uint16_t num = strtol(input + i, &end, 10);
    // normalize
    uint8_t normalized = (double) num / header.max_value * UINT8_MAX;

    i = end - input;
    *(uint8_t *) ((uint8_t *) pixels + pixels_pos++) = normalized;
  }

  PixMap3Image *map = (PixMap3Image *) pixels;
  map->header = header;

  uint8_t *pix = map->pixels;
  return map;
}
