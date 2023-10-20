#pragma once

#include "color_converter/cmyk.h"
#include "color_converter/rgb.h"
#include "gtk/gtk.h"

typedef struct {
  ColorRgb **rgb;
  ColorCmyk **cmyk;
  GtkPicture **image;
  GdkPixbuf **pixbuf;
} ColorsContext;

ColorsContext *colors_context_new();

void colors_context_free(ColorsContext *context);

void colors_context_color_update(ColorsContext *context);

// 💀 brooooo
#define GET_CONTEXT singleton_context(NULL)
ColorsContext *singleton_context(ColorsContext *context);
