#include "drawing_app/window.h"

void drawing_app_activate(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);

  gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
  gtk_window_set_title(GTK_WINDOW(window), "Drawing app");

  gtk_widget_set_visible(window, TRUE);
}
