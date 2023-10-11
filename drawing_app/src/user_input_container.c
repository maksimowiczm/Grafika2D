#include "drawing_app/widgets/user_input_container.h"


static GtkWidget *
new_point_input(Context *context, int index) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  *(context->user_input.inputs + index) = container;

  GtkWidget *input = gtk_entry_new();
  char i = '0' + (char) index;
  char label_text[10] = "Point ";
  strncat(label_text, &i, 1);
  GtkWidget *label = gtk_label_new(label_text);
  gtk_box_append(GTK_BOX(container), label);
  gtk_box_append(GTK_BOX(container), input);
  gtk_widget_is_visible(input);

  return container;
}

inline static gboolean
handle_draw_click(GtkWidget *button, gpointer user_data) {
  Context *context = user_data;
  return handle_draw_button_click(context);
}

GtkWidget *new_user_input_container(Context *context) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

  for (int i = 0; i < context->user_input.inputs_length; i++) {
    gtk_box_append(GTK_BOX(container), new_point_input(context, i));
  }

  GtkWidget *draw_button = gtk_button_new_with_label("Draw");
  g_signal_connect(draw_button, "clicked", G_CALLBACK(handle_draw_click), context);

  context->user_input.draw_button = draw_button;
  gtk_box_append(GTK_BOX(container), draw_button);

  return container;
}
