#include "color_ui/internal/color_container.h"

static gboolean
state_set(
    GtkSwitch *self,
    gboolean state,
    gpointer user_data
) {
  GET_CONTEXT->RGB_MODE = !state;
  GtkLabel *label = user_data;
  if (state) {
    gtk_label_set_text(label, "CMYK");
  } else {
    gtk_label_set_text(label, "RGB");
  }
  return FALSE;
}

GtkWidget *color_container_new(ColorsContext *context) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  GtkWidget *switch_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget *mode_switch = gtk_switch_new();
  GtkWidget *label = gtk_label_new("RGB");
  gtk_box_append(GTK_BOX(switch_container), mode_switch);
  gtk_box_append(GTK_BOX(switch_container), label);
  gtk_box_append(GTK_BOX(container), switch_container);
  g_signal_connect(mode_switch, "state-set", G_CALLBACK(state_set), label);

  GtkWidget *image = gtk_picture_new();
  gtk_box_append(GTK_BOX(container), image);
  gtk_widget_set_hexpand(image, TRUE);
  gtk_widget_set_vexpand(image, TRUE);
  *context->image = GTK_PICTURE(image);

  return container;
}
