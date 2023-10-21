#include "headers.h"
#include "cube.h"

static void
onPixbufDestroy(guchar *pixels, gpointer data) {
  free(pixels);
}

static void
add_wall(GtkWidget *container, uint8_t *(*get_wall)()) {
  uint8_t *wall = get_wall();
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(
      wall,
      GDK_COLORSPACE_RGB,
      FALSE,
      8,
      X_LENGTH,
      Y_LENGTH,
      X_LENGTH * 3,
      onPixbufDestroy,
      NULL
  );
  GtkWidget *picture = gtk_picture_new();
  gtk_picture_set_pixbuf(GTK_PICTURE(picture), pixbuf);
  gtk_box_append(GTK_BOX(container), picture);
  gtk_widget_set_hexpand(picture, TRUE);
  gtk_widget_set_vexpand(picture, TRUE);
}

void window_activate(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  GtkWidget *app_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
  gtk_window_set_child(GTK_WINDOW(window), app_container);

  GtkWidget *first = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
  gtk_box_append(GTK_BOX(app_container), first);
  add_wall(first, get_top_wall);
  add_wall(first, get_blank_wall);
  add_wall(first, get_blank_wall);
  add_wall(first, get_blank_wall);

  GtkWidget *second = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
  gtk_box_append(GTK_BOX(app_container), second);
  add_wall(second, get_front_wall);
  add_wall(second, get_side_wall);
  add_wall(second, get_prime_front_wall);
  add_wall(second, get_prime_side_wall);

  GtkWidget *third = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
  gtk_box_append(GTK_BOX(app_container), third);
  add_wall(third, get_prime_top_wall);
  add_wall(third, get_blank_wall);
  add_wall(third, get_blank_wall);
  add_wall(third, get_blank_wall);

  gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
  gtk_window_set_title(GTK_WINDOW(window), "Drawing app");

  gtk_widget_set_visible(window, TRUE);
}