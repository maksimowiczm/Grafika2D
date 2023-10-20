#include "color_ui/internal/window.h"
#include "color_ui/internal/rgb_container.h"

#include "color_converter/rgb.h"
#include "color_converter/cmyk.h"
#include "color_ui/internal/cmyk_container.h"

typedef struct {
  ColorRgb *rgb;
  ColorCmyk *cmyk;
} Colors;

static void
on_destroy(GtkWidget *window, gpointer user_data) {
  Colors *colors = user_data;
  free(colors->cmyk);
  free(colors->rgb);
  free(colors);
}

void window_activate(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  GtkWidget *app_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_window_set_child(GTK_WINDOW(window), app_container);

  ColorRgb *rgb = malloc(sizeof(*rgb));
  memset(rgb, 0, sizeof(*rgb));
  GtkWidget *rgb_container = rgb_container_new(rgb);
  gtk_box_append(GTK_BOX(app_container), rgb_container);

  ColorCmyk *cmyk = malloc(sizeof(*cmyk));
  memset(cmyk, 0, sizeof(*cmyk));
  GtkWidget *cmyk_container = cmyk_container_new(cmyk);
  gtk_box_append(GTK_BOX(app_container), cmyk_container);

  Colors *colors = malloc(sizeof(*colors));
  colors->rgb = rgb;
  colors->cmyk = cmyk;

  gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
  gtk_window_set_title(GTK_WINDOW(window), "Drawing app");

  g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), colors);

  gtk_widget_set_visible(window, TRUE);
}
