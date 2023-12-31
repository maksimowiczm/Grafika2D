#include "headers.h"
#include "cube.h"
#include "cube_state/cube_context.h"

static void
on_left_button_click(GtkWidget *button, gpointer user_data) {
  CubeContext *context = user_data;
  cube_context_left(context);
}

static void
on_right_button_click(GtkWidget *button, gpointer user_data) {
  CubeContext *context = user_data;
  cube_context_right(context);
}

static void
on_destroy(GtkWidget *widget, gpointer user_data) {
  cube_context_free(user_data);
}

static void
on_flip_button_click(GtkWidget *widget, gpointer user_data) {
  cube_context_flip(user_data);
}

void window_activate(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  GtkWidget *app_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
  gtk_window_set_child(GTK_WINDOW(window), app_container);

  GtkWidget *picture = gtk_picture_new();
  CubeContext *context = cube_context_new(GTK_PICTURE(picture));
  gtk_widget_set_hexpand(picture, TRUE);
  gtk_widget_set_vexpand(picture, TRUE);

  GtkWidget *middle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_box_append(GTK_BOX(app_container), middle);

  GtkWidget *left_button = gtk_button_new_with_label("left");
  g_signal_connect(left_button, "clicked", G_CALLBACK(on_left_button_click), context);
  gtk_box_append(GTK_BOX(middle), left_button);

  gtk_box_append(GTK_BOX(middle), picture);

  GtkWidget *right_button = gtk_button_new_with_label("right");
  g_signal_connect(right_button, "clicked", G_CALLBACK(on_right_button_click), context);
  gtk_box_append(GTK_BOX(middle), right_button);

  GtkWidget *flip_button = gtk_button_new_with_label("flip");
  g_signal_connect(flip_button, "clicked", G_CALLBACK(on_flip_button_click), context);
  gtk_box_append(GTK_BOX(app_container), flip_button);

  gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
  gtk_window_set_title(GTK_WINDOW(window), "Drawing app");

  gtk_widget_set_visible(window, TRUE);
  g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), context);
}
