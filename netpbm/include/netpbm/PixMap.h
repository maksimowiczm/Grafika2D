#pragma once

#include "inttypes.h"

struct pix_map_header {
  uint16_t width;
  uint16_t height;
  uint16_t max_value;
};

typedef struct pix_map_header PixMapHeader;

struct pix_map {
  PixMapHeader header;
  uint8_t pixels[];
};

typedef struct pix_map PixMapImage;
