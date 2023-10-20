#include "color_ui/internal/cmyk_container.h"
#include "color_ui/internal/uint8_picker.h"

GtkWidget *cmyk_container_new(ColorCmyk *color) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("cyan", &color->cyan, 100));
  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("magenta", &color->magenta, 100));
  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("yellow", &color->yellow, 100));
  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("black", &color->key, 100));

  return container;
}
