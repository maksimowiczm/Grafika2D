#include "drawing_app/widgets/drawable_area.h"


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
    bool isDrawn = shape->header.isDrawn;
    if (shouldBeDrawn && !isDrawn) {
      shape->header.draw_method(*shape, cr, true);
      shape->header.isDrawn = true;
    }
  }
}


typedef struct {
  WindowState *state;
  GtkWidget *drawingArea;
} ClickData;

static gboolean
left_clicked(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data) {
  ClickData *clickData = data;

  // do smth todo

  gtk_widget_queue_draw(clickData->drawingArea);

  return TRUE;
}


static void
destroy_handler(GtkWidget *widget, gpointer data) {
  free(data);
}


GtkWidget *new_drawable_area(WindowState *state) {
  GtkWidget *area = gtk_drawing_area_new();

  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA (area), (GtkDrawingAreaDrawFunc) do_drawing, state, NULL);
  gtk_widget_set_size_request(area, 1000, 0); // todo
  gtk_widget_set_halign(area, GTK_ALIGN_FILL);
  gtk_widget_set_valign(area, GTK_ALIGN_FILL);

  // left click
  GtkGesture *gesture_click = gtk_gesture_click_new();
  gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture_click), 1);
  gtk_widget_add_controller(area, GTK_EVENT_CONTROLLER (gesture_click));
  ClickData *clickData = malloc(sizeof(*clickData));
  clickData->state = state;
  clickData->drawingArea = area;
  g_signal_connect(gesture_click, "pressed", G_CALLBACK(left_clicked), clickData);
  g_signal_connect(area, "destroy", G_CALLBACK(destroy_handler), clickData);

  return area;
}
