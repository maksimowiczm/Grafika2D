#include "image_gui_gtk/app.h"

void gui_gtk_app_activate(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);

  gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
  gtk_window_set_title(GTK_WINDOW(window), "Images app");

  GtkWidget *app_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_window_set_child(GTK_WINDOW(window), app_container);

  gtk_widget_set_visible(window, TRUE);
}
