#include "color_ui/internal/rgb_container.h"
#include "color_ui/internal/uint8_picker.h"

GtkWidget *rgb_container_new(ColorRgb *color) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("red", &color->red, 255));
  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("green", &color->green, 255));
  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("blue", &color->blue, 255));

  return container;
}
