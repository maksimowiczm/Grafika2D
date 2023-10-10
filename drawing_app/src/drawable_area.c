#include "drawing_app/widgets/drawable_area.h"
#include "drawio/cairo.h"

#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define BLUE 0, 0, 255

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
      shape->header.draw_method(*shape, cr, true);
      shape->header.isDrawn = true;
    }
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


typedef struct {
  WindowState *state;
  GtkWidget *drawingArea;
} ClickData;


static gboolean
handle_draw(WindowState *state, double x, double y) {
  PointBuffer *buffer = &state->buffer;

  // add points to state buffer
  if (buffer->buffer_current_size + 1 > buffer->buffer_size) {
    state_buffer_clear(state);
    state_redraw(state);
    return TRUE;
  }
  state_buffer_add(state, (Point) {x, y});

  // handle shape creation
  if (state->buffer.buffer_current_size >= shapes_point_count_to_create(state->currentType)) {
    state_shapes_add(state);
  }

  state_redraw(state);
  return TRUE;
}


static gboolean
handle_move(WindowState *state, double x, double y) {
  state_moving_point_move(state, (Point) {x, y});
  state_redraw(state);
  return TRUE;
}


static gboolean
left_clicked(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data) {
  WindowState *state = data;

  if (state->action == NoAction || state->action == Drawing) {
    return handle_draw(state, x, y);
  } else if (state->action == MovingPoint) {
    return handle_move(state, x, y);
  }

  return TRUE;
}


static gboolean
right_clicked(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data) {
  WindowState *state = data;

  if (state->action == MovingPoint) {
    state_buffer_clear(state);
    state->action = NoAction;
    state_redraw(state);
    return TRUE;
  }

  DrawableShape *shape = state_shapes_closest_shape(state, (Point) {x, y});
  if (shape == NULL) {
    return TRUE;
  }
  state_buffer_clear(state);

  Point *closest = shapes_shape_closest_point(shape->shape, (Point) {x, y});
  state_moving_point_set(state, closest);

  state_redraw(state);

  return TRUE;
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

  return area;
}
