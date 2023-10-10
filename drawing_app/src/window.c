#include "drawing_app/window.h"


#include "drawing_app/widgets/main_container.h"
#include "drawio/draw.h"
#include "drawio/draw_methods.h"
#include "drawing_app/context.h"

static void
destroy_handler(GtkWidget *widget, gpointer data) {
  Context *context = data;
  context_free(context, true);
}

static void
context_populate_shapes(Context *context) {
  Shape *rectangle = shapes_new_shape(Rectangle, (Point[2]) {10, 10, 400, 400});
  DrawableShape *drawableRectangle = drawio_new_drawableShape(rectangle, drawio_rectangle_method);
  *context->shapes = drawableRectangle;

  Shape *line = shapes_new_shape(Line, (Point[2]) {10, 10, 500, 500});
  DrawableShape *drawableLine = drawio_new_drawableShape(line, drawio_loop_method);
  *(context->shapes + 1) = drawableLine;

  Shape *circle = shapes_new_shape(Circle, (Point[2]) {400, 400, 450, 400});
  DrawableShape *drawableCircle = drawio_new_drawableShape(circle, drawio_circle_method);
  *(context->shapes + 2) = drawableCircle;
}

void drawing_app_activate(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  Context *context = context_new(1000, 10);
  context_populate_shapes(context);

  gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
  gtk_window_set_title(GTK_WINDOW(window), "Drawing app");

  GtkWidget *container = new_main_container(context);
  gtk_window_set_child(GTK_WINDOW(window), container);

  g_signal_connect(window, "destroy", G_CALLBACK(destroy_handler), context);

  gtk_widget_set_visible(window, TRUE);
}
