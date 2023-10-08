#include "gtk/gtk.h"

#define APP_ACTIVATE(name) void(*name)(GtkApplication *)

int my_app_start(int argc, char *argv[], APP_ACTIVATE(activate_app)) {
  GtkApplication *app = gtk_application_new("org.gtk.grafika", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate_app), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}

int main(int argc, char *argv[]) {
  return my_app_start(argc, argv, NULL);
}
