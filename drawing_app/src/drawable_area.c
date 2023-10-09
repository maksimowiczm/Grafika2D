#include "drawing_app/widgets/drawable_area.h"
#include "drawio/cairo.h"


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

  cairo_set_source_rgb(cr, 255, 0, 0);
  drawio_points_mark(cr, state->buffer.buffer, state->buffer.buffer_current_size);
}


typedef struct {
  WindowState *state;
  GtkWidget *drawingArea;
} ClickData;

static gboolean
left_clicked(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data) {
  WindowState *state = data;
  PointBuffer *buffer = &state->buffer;

  // add points to state buffer
  if (buffer->buffer_current_size + 1 > buffer->buffer_size) {
    state_buffer_clear(state);
    gtk_widget_queue_draw(state->drawing_area);
    return TRUE;
  }
  state_buffer_add(state, (Point) {x, y});

  // handle shape creation
  if (state->buffer.buffer_current_size >= shapes_point_count_to_create(state->currentType)) {
    state_shapes_add(state);
  }

  gtk_widget_queue_draw(state->drawing_area);

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

  return area;
}
