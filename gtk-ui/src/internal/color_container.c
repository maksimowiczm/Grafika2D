#include "color_ui/internal/color_container.h"

GtkWidget *color_container_new(ColorsContext *context) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  GtkWidget *image = gtk_picture_new();
  gtk_box_append(GTK_BOX(container), image);
  gtk_widget_set_hexpand(image, TRUE);
  gtk_widget_set_vexpand(image, TRUE);
  *context->image = GTK_PICTURE(image);

  return container;
}
