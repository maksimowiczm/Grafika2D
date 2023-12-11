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
                   int64_t image_width,
                   int64_t image_height,
                   int64_t x,
                   int64_t y,
                   const uint8_t *mask,
                   int64_t mask_width,
                   int64_t mask_height);