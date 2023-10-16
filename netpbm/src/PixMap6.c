#include "netpbm/PixMap6.h"

#include "string.h"
#include "stdlib.h"

PixMap6Image *netpbm_P6_read_from_string(const char *input, size_t length) {
  size_t headerEnd = 0;
  PixMapHeader header;
  netpbm_pix_map_read_header(&header, &headerEnd, input, length);
  if (headerEnd == 0) {
    return NULL;
  }

  size_t pixels_count = header.height * header.width * 3;
  void *pixels = malloc(sizeof(PixMapHeader) + sizeof(uint8_t) * pixels_count);
  memset(pixels, 0, sizeof(PixMapHeader) + sizeof(uint8_t) * pixels_count);

  memcpy(pixels + sizeof(PixMapHeader), input + headerEnd, pixels_count);

  PixMap6Image *map = (PixMap6Image *) pixels;
  map->header = header;

  return map;
}
