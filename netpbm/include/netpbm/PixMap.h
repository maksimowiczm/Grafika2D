#pragma once

#include "inttypes.h"

struct pix_map {
  uint16_t width;
  uint16_t height;
  uint16_t max_value;
  uint16_t pixels[];
};

typedef struct pix_map PixMapImage;
