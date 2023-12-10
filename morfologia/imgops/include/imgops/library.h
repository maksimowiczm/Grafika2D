#pragma once
#include "stdbool.h"
#include "inttypes.h"
#include "stdlib.h"

enum {
  ERROR = -1,
  MISS,
  HIT,
  FIT
};

int8_t
imgops_hit_or_miss(const uint8_t *image,
                   uint64_t image_width,
                   uint64_t image_height,
                   uint64_t x,
                   uint64_t y,
                   const uint8_t *mask,
                   uint64_t mask_width,
                   uint64_t mask_height);