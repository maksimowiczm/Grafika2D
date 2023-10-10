#include "drawing_app/window.h"

#include "drawing_app/widgets/main_container.h"
#include "drawio/draw.h"
#include "drawing_app/context.h"


static void
destroy_handler(GtkWidget *widget, gpointer data) {
  Context *context = data;
  context_free(context, true);
}

void drawing_app_activate(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  Context *context = context_new(1000, 10);

  gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
  gtk_window_set_title(GTK_WINDOW(window), "Drawing app");

  GtkWidget *container = new_main_container(context);
  gtk_window_set_child(GTK_WINDOW(window), container);

  g_signal_connect(window, "destroy", G_CALLBACK(destroy_handler), context);

  gtk_widget_set_visible(window, TRUE);
}
