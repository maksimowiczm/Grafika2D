#include "gtk/gtk.h"

extern "C" {
#include "image_gui_gtk/app.h"
}

int gtk_app_start(int argc, char *argv[]) {
  GtkApplication *app = gtk_application_new("org.gtk.grafika", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(gui_gtk_app_activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}

int main(int argc, char *argv[]) {
  return gtk_app_start(argc, argv);
}
