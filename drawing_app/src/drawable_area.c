#include "drawing_app/widgets/drawable_area.h"
#include "drawio/cairo.h"

#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define BLUE 0, 0, 255
#define BLACK 0, 0, 0

static void
do_drawing(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data) {
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1);

  WindowState *state = data;

  for (int i = 0; i < state->shapes_length; i++) {
    DrawableShape *shape = state->shapes[i];
    if (shape == NULL) {
      continue;
    }
    bool shouldBeDrawn = shape->header.shouldBeDrawn;
    if (shouldBeDrawn) {
      shape->header.draw_method(*shape, cr, true, BLACK);
      shape->header.isDrawn = true;
    }
  }

  // mark moving shape
  if (state->action == MovingShape) {
    DrawableShape *shape = *state->moving_shape;
    shape->header.draw_method(*shape, cr, true, RED);
    drawio_points_mark(cr, shape->shape->points, shape->shape->points_length, RED);
    return;
  }

  // mark buffer
  drawio_points_mark(cr, state->buffer.buffer, state->buffer.buffer_current_size, RED);

  // mark shapes
  for (int i = 0; i < state->shapes_length; i++) {
    if (state->shapes[i] == NULL) {
      continue;
    }
    Point *points = state->shapes[i]->shape->points;
    size_t length = state->shapes[i]->shape->points_length;
    drawio_points_mark(cr, points, length, GREEN);
  }

  // mark moving point
  if (state->action == MovingPoint) {
    drawio_points_mark(cr, state->moving_point, 1, BLUE);
  }
}


static gboolean
left_clicked(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data) {
  WindowState *state = data;
  return state_handle_left_click(state, (Point) {x, y});
}


static gboolean
right_clicked(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data) {
  WindowState *state = data;
  state_handle_right_click(state, (Point) {x, y});
}


static gboolean
long_right_click(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data) {
  WindowState *state = data;
  if (state->action == MovingShape) {
    return FALSE;
  }

  DrawableShape *shape = state_shapes_closest_shape(state, (Point) {x, y});
  if (shape == NULL) {
    return TRUE;
  }
  state_moving_shape_set(state, shape, (Point) {x, y});
  state_redraw(state);

  return TRUE;
}


static void
mouse_movement(GtkEventControllerMotion *self, gdouble x, gdouble y, gpointer user_data) {
  WindowState *state = user_data;
  if (state->action != MovingShape) {
    return;
  }

  Vector2D vector = {-(state->previous_moving_shape_position.x - x),
                     -(state->previous_moving_shape_position.y - y)};
  state->previous_moving_shape_position.x = x;
  state->previous_moving_shape_position.y = y;

  Shape *shape = (*state->moving_shape)->shape;
  shapes_shape_move(shape, vector);
  state_redraw(state);
}


GtkWidget *new_drawable_area(WindowState *state) {
  GtkWidget *area = gtk_drawing_area_new();
  state->drawing_area = area;

  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA (area), (GtkDrawingAreaDrawFunc) do_drawing, state, NULL);
  gtk_widget_set_size_request(area, 1000, 0); // todo
  gtk_widget_set_halign(area, GTK_ALIGN_FILL);
  gtk_widget_set_valign(area, GTK_ALIGN_FILL);

  // left click
  GtkGesture *gesture_click = gtk_gesture_click_new();
  gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture_click), 1);
  gtk_widget_add_controller(area, GTK_EVENT_CONTROLLER (gesture_click));
  g_signal_connect(gesture_click, "pressed", G_CALLBACK(left_clicked), state);


  // right click
  GtkGesture *gesture_right_click = gtk_gesture_click_new();
  gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture_right_click), 3);
  gtk_widget_add_controller(area, GTK_EVENT_CONTROLLER (gesture_right_click));
  g_signal_connect(gesture_right_click, "pressed", G_CALLBACK(right_clicked), state);


  // real
  GtkGesture *gesture_long_press = gtk_gesture_long_press_new();
  gtk_gesture_long_press_set_delay_factor(GTK_GESTURE_LONG_PRESS(gesture_long_press), .5);
  gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture_long_press), 3);
  gtk_widget_add_controller(area, GTK_EVENT_CONTROLLER (gesture_long_press));
  g_signal_connect(gesture_long_press, "pressed", G_CALLBACK(long_right_click), state);

  GtkEventController *controller = gtk_event_controller_motion_new();
  gtk_widget_add_controller(area, GTK_EVENT_CONTROLLER(controller));
  g_signal_connect(controller, "motion", G_CALLBACK(mouse_movement), state);

  return area;
}
