#include "drawing_app/widgets/side_menu.h"
#include "drawing_app/widgets/user_input_container.h"


#define SHAPE_CLICKED(shape, name) \
static gboolean \
name(GtkWidget *gesture, gpointer user_data) { \
  Context* context = user_data; \
  context_set_shape(context, shape); \
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
button_clear_click(GtkWidget *button, gpointer user_data) {
  Context *context = user_data;
  context_clear_all(context);
  context_redraw(context);
  return TRUE;
}


GtkWidget *new_side_menu(Context *context) {
  GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

  GtkWidget *line = shape_button_new("line", line_click, context);
  gtk_box_append(GTK_BOX(container), line);

  GtkWidget *rectangle = shape_button_new("rectangle", rectangle_click, context);
  gtk_box_append(GTK_BOX(container), rectangle);

  GtkWidget *quadrilateral = shape_button_new("quadrilateral", quadrilateral_click, context);
  gtk_box_append(GTK_BOX(container), quadrilateral);

  GtkWidget *circle = shape_button_new("circle", circle_click, context);
  gtk_box_append(GTK_BOX(container), circle);

  GtkWidget *clear_button = gtk_button_new_with_label("clear");
  g_signal_connect(clear_button, "clicked", G_CALLBACK(button_clear_click), context);
  gtk_box_append(GTK_BOX(container), clear_button);

  GtkWidget *user_input = new_user_input_container(context);
  gtk_box_append(GTK_BOX(container), user_input);

  return container;
}
