#include "color_ui/internal/rgb_container.h"
#include "color_ui/internal/uint8_picker.h"

GtkWidget *rgb_container_new(ColorsContext *context) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

  ColorRgb *rgb = *context->rgb;
  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("red", &rgb->red, 255, RGB, RED));
  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("green", &rgb->green, 255, RGB, GREEN));
  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("blue", &rgb->blue, 255, RGB, BLUE));

  return container;
}
