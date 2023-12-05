#pragma once

#include "color_converter/cmyk.h"
#include "color_converter/rgb.h"
#include "gtk/gtk.h"

enum ColorIndex {
  RED,
  GREEN,
  BLUE,
  CYAN,
  MAGENTA,
  YELLOW,
  KEY
};

typedef struct {
  ColorRgb **rgb;
  ColorCmyk **cmyk;
  GtkPicture **image;
  GdkPixbuf **pixbuf;

  GtkWidget **scales;
  GtkWidget **inputs;
  bool RGB_MODE;
} ColorsContext;

ColorsContext *colors_context_new();

void colors_context_free(ColorsContext *context);

enum ColorMode {
  RGB, CMYK
};
void colors_context_color_update(ColorsContext *context, enum ColorMode);

// 💀 brooooo
#define GET_CONTEXT singleton_context(NULL)

ColorsContext *singleton_context(ColorsContext *context);
