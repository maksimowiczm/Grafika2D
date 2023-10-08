#include "drawing_app/window.h"


#include "drawing_app/widgets/main_container.h"
#include "drawio/draw.h"
#include "drawio/draw_methods.h"

static void
destroy_handler(GtkWidget *widget, gpointer data) {
  WindowState *state = data;

  for (int i = 0; i < state->shapes_length; i++) {
    DrawableShape *shape = state->shapes[i];
    if (shape == NULL) {
      continue;
    }

    // free inner shape
    shapes_shape_free(shape->shape, true);

    // free outer shape
    drawio_drawableShape_free(shape, true);
  }

  // free state
  free(state->shapes);
  free(state);
}

static void
state_populate_shapes(WindowState *state) {
  Shape *rectangle = shapes_new_shape(Rectangle, (Point[2]) {10, 10, 400, 400});
  DrawableShape *drawableRectangle = drawio_new_drawableShape(rectangle, drawio_loop_method);
  *state->shapes = drawableRectangle;

  Shape *line = shapes_new_shape(Line, (Point[2]) {10, 10, 500, 500});
  DrawableShape *drawableLine = drawio_new_drawableShape(line, drawio_loop_method);
  *(state->shapes + 1) = drawableLine;

  Shape *circle = shapes_new_shape(Circle, (Point[2]) {400, 400, 450, 400});
  DrawableShape *drawableCircle = drawio_new_drawableShape(circle, drawio_circle_method);
  *(state->shapes + 2) = drawableCircle;
}

static void
state_initialize(WindowState *state, size_t shape_max_size) {
  state->shapes_length = shape_max_size;
  state->shapes = malloc(sizeof(DrawableShape **) * state->shapes_length);
  memset(state->shapes, 0, sizeof(DrawableShape **) * state->shapes_length);

  state_populate_shapes(state);
}

void drawing_app_activate(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  WindowState *state = malloc(sizeof(*state));
  state_initialize(state, 1000);

  gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
  gtk_window_set_title(GTK_WINDOW(window), "Drawing app");

  GtkWidget *container = new_main_container(state);
  gtk_window_set_child(GTK_WINDOW(window), container);

  g_signal_connect(window, "destroy", G_CALLBACK(destroy_handler), state);

  gtk_widget_set_visible(window, TRUE);
}
