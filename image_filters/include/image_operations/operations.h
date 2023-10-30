#pragma once

#include "inttypes.h"
#include "stdlib.h"

#define IMAGE_OPERATIONS_CHANNELS_RGB 3
#define IMAGE_OPERATIONS_CHANNELS_ALL 1

#define IMAGE_OPERATIONS_CHANNEL_ALL 0
#define IMAGE_OPERATIONS_CHANNEL_RED 0
#define IMAGE_OPERATIONS_CHANNEL_GREEN 1
#define IMAGE_OPERATIONS_CHANNEL_BLUE 2

void image_operations_add(uint8_t *pixels,
                          size_t pixels_length,
                          size_t channels_count,
                          size_t channel,
                          int16_t value);
