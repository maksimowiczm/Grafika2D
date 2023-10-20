#include "color_ui/internal/colors_context.h"

#include "stdlib.h"
#include "string.h"

ColorsContext *colors_context_new() {
  ColorsContext *context = malloc(sizeof(*context));
  memset(context, 0, sizeof(*context));
  context->rgb = malloc(sizeof(ColorRgb *));
  context->cmyk = malloc(sizeof(ColorCmyk *));

  ColorRgb *rgb = malloc(sizeof(*rgb));
  ColorCmyk *cmyk = malloc(sizeof(*cmyk));
  *context->rgb = rgb;
  *context->cmyk = cmyk;

  return context;
}

void colors_context_free(ColorsContext *context) {
  free(*context->rgb);
  free(*context->cmyk);

  free(context->cmyk);
  free(context->rgb);

  free(context);
}
