#include "drawing_app/widgets/side_menu.h"


#define SHAPE_CLICKED(shape, name) \
static gboolean \
name(GtkWidget *gesture, gpointer data) { \
  WindowState *state = data; \
  state_shape_choose(state, shape); \
  return TRUE; \
}

SHAPE_CLICKED(Line, line_click)

SHAPE_CLICKED(Quadrilateral, quadrilateral_click)

SHAPE_CLICKED(Circle, circle_click)

SHAPE_CLICKED(Rectangle, rectangle_click)


static GtkWidget *
shape_button_new(const char *label,
                 gboolean(*callback)(GtkWidget *, gpointer),
                 gpointer data) {
  GtkWidget *button = gtk_button_new_with_label(label);
  g_signal_connect(button, "clicked", G_CALLBACK(callback), data);

  return button;
}


static gboolean
button_clear_click(GtkWidget *button, gpointer data) {
  WindowState *state = data;
  state_shapes_clear(state);
  state_buffer_clear(state);
  state_redraw(state);
  return TRUE;
}


GtkWidget *new_side_menu(WindowState *state) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

  GtkWidget *line = shape_button_new("line", line_click, state);
  gtk_box_append(GTK_BOX(container), line);

  GtkWidget *rectangle = shape_button_new("rectangle", rectangle_click, state);
  gtk_box_append(GTK_BOX(container), rectangle);

  GtkWidget *quadrilateral = shape_button_new("quadrilateral", quadrilateral_click, state);
  gtk_box_append(GTK_BOX(container), quadrilateral);

  GtkWidget *circle = shape_button_new("circle", circle_click, state);
  gtk_box_append(GTK_BOX(container), circle);

  GtkWidget *clear_button = gtk_button_new_with_label("clear");
  g_signal_connect(clear_button, "clicked", G_CALLBACK(button_clear_click), state);
  gtk_box_append(GTK_BOX(container), clear_button);

  return container;
}
