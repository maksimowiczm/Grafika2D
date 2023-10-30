#include "color_ui/app.h"
#include "color_ui/internal/window.h"

#include "gtk/gtk.h"

int color_ui_app_start(int argc, char **argv) {
  GtkApplication *app = gtk_application_new("org.gtk.grafika", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(window_activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
