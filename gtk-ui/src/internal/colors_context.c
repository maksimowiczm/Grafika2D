#include "color_ui/internal/colors_context.h"

#include "stdlib.h"
#include "string.h"

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

void colors_context_color_update(ColorsContext *context) {
  if (context == NULL) {
    return;
  }
  const guchar buffer[3] = {(*context->rgb)->red, (*context->rgb)->green, (*context->rgb)->blue};

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
