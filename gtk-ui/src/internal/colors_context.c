#include "color_ui/internal/colors_context.h"

#include "stdlib.h"
#include "string.h"
#include "color_converter/cmyk_converter.h"

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

  singleton_context(context);

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
  guchar buffer[3];
  if (mode == RGB) {
    buffer[0] = (*context->rgb)->red;
    buffer[1] = (*context->rgb)->green;
    buffer[2] = (*context->rgb)->blue;
  } else if (mode == CMYK) {
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
}
