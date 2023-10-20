#pragma once

#include "color_converter/cmyk.h"
#include "color_converter/rgb.h"

typedef struct {
  ColorRgb **rgb;
  ColorCmyk **cmyk;
} ColorsContext;

ColorsContext *colors_context_new();

void colors_context_free(ColorsContext* context);
