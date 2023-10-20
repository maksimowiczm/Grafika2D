#include "color_ui/internal/colors_context.h"

#include "stdlib.h"
#include "string.h"
#include "color_converter/cmyk_converter.h"
#include "color_converter/rgb_converter.h"
#include "shared/uint8.h"

ColorsContext *singleton_context(ColorsContext *context) {
  static ColorsContext *singleton;
  if (context == NULL) {
    return singleton;
  }

  singleton = context;
}

ColorsContext *colors_context_new() {
  ColorsContext *context = malloc(sizeof(*context));
  memset(context, 0, sizeof(*context));
  context->rgb = malloc(sizeof(ColorRgb *));
  context->cmyk = malloc(sizeof(ColorCmyk *));

  ColorRgb *rgb = malloc(sizeof(*rgb));
  ColorCmyk *cmyk = malloc(sizeof(*cmyk));
  *context->rgb = rgb;
  *context->cmyk = cmyk;

  context->image = malloc(sizeof(GtkImage *));
  context->pixbuf = malloc(sizeof(GdkPixbuf *));

  context->inputs = malloc(sizeof(GtkText *) * 7);
  context->scales = malloc(sizeof(GtkScale *) * 7);

  singleton_context(context);

  context->RGB_MODE = false;

  return context;
}

void colors_context_free(ColorsContext *context) {
  free(*context->rgb);
  free(*context->cmyk);

  free(context->cmyk);
  free(context->rgb);

  free(context->image);
  free(context->pixbuf);

  free(context);
}

void colors_context_color_update(ColorsContext *context, enum ColorMode mode) {
  if (context == NULL) {
    return;
  }
  if ((context->RGB_MODE && mode == CMYK) || (!context->RGB_MODE && mode == RGB)) {
    return;
  }

  guchar buffer[3];
  if (context->RGB_MODE) {
    buffer[0] = (*context->rgb)->red;
    buffer[1] = (*context->rgb)->green;
    buffer[2] = (*context->rgb)->blue;
  } else {
    ColorCmyk *contextCmyk = *context->cmyk;
    ColorRgb rgb = color_converter_cmyk2rgb(*contextCmyk);

    buffer[0] = rgb.red;
    buffer[1] = rgb.green;
    buffer[2] = rgb.blue;
  }

  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(
      buffer,
      GDK_COLORSPACE_RGB,
      FALSE,
      8,
      1,
      1,
      3,
      NULL,
      NULL
  );
  pixbuf = gdk_pixbuf_scale_simple(pixbuf, 3840, 2160, GDK_INTERP_NEAREST);
  gtk_picture_set_pixbuf(*context->image, pixbuf);

  if (context->RGB_MODE) {
    ColorRgb *rgb = *context->rgb;
    ColorCmyk cmyk = color_converter_rgb2cmyk(*rgb);
    (*context->cmyk)->cyan = cmyk.cyan;
    (*context->cmyk)->magenta = cmyk.magenta;
    (*context->cmyk)->yellow = cmyk.yellow;
    (*context->cmyk)->key = cmyk.key;

    gtk_entry_buffer_set_text(gtk_text_get_buffer(context->inputs[CYAN]), uint8_to_string(cmyk.cyan), 3);
    gtk_range_set_value(context->scales[CYAN], cmyk.cyan);

    gtk_entry_buffer_set_text(gtk_text_get_buffer(context->inputs[MAGENTA]), uint8_to_string(cmyk.magenta), 3);
    gtk_range_set_value(context->scales[MAGENTA], cmyk.magenta);

    gtk_entry_buffer_set_text(gtk_text_get_buffer(context->inputs[YELLOW]), uint8_to_string(cmyk.yellow), 3);
    gtk_range_set_value(context->scales[YELLOW], cmyk.yellow);

    gtk_entry_buffer_set_text(gtk_text_get_buffer(context->inputs[KEY]), uint8_to_string(cmyk.key), 3);
    gtk_range_set_value(context->scales[KEY], cmyk.key);
  } else {
    ColorCmyk *cmyk = *context->cmyk;
    ColorRgb rgb = color_converter_cmyk2rgb(*cmyk);
    (*context->rgb)->red = rgb.red;
    (*context->rgb)->green = rgb.green;
    (*context->rgb)->blue = rgb.blue;

    gtk_entry_buffer_set_text(gtk_text_get_buffer(context->inputs[RED]), uint8_to_string(rgb.red), 3);
    gtk_range_set_value(context->scales[RED], rgb.red);

    gtk_entry_buffer_set_text(gtk_text_get_buffer(context->inputs[GREEN]), uint8_to_string(rgb.green), 3);
    gtk_range_set_value(context->scales[GREEN], rgb.green);

    gtk_entry_buffer_set_text(gtk_text_get_buffer(context->inputs[BLUE]), uint8_to_string(rgb.blue), 3);
    gtk_range_set_value(context->scales[BLUE], rgb.blue);
  }
}