#include "color_ui/internal/cmyk_container.h"
#include "color_ui/internal/uint8_picker.h"

GtkWidget *cmyk_container_new(ColorsContext *context) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

  ColorCmyk *cmyk = *context->cmyk;
  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("cyan", &cmyk->cyan, 100, CMYK, CYAN));
  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("magenta", &cmyk->magenta, 100, CMYK, MAGENTA));
  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("yellow", &cmyk->yellow, 100, CMYK, YELLOW));
  gtk_box_append(GTK_BOX(container), gtk_ui_uint8_picker("black", &cmyk->key, 100, CMYK, KEY));

  return container;
}
