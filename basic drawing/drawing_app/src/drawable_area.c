#include "drawing_app/widgets/drawable_area.h"
#include "drawio/cairo.h"


#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define BLUE 0, 0, 255
#define BLACK 0, 0, 0

static void
do_drawing(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1);
  context_draw(user_data, cr);
}

inline static gboolean
left_clicked(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data) {
  return context_handle_left_click(user_data, (Point) {x, y});
}

inline static gboolean
right_clicked(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data) {
  return context_handle_right_click(user_data, (Point) {x, y});
}

inline static gboolean
long_right_click(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data) {
  return context_handle_right_click_long(user_data, (Point) {x, y});
}

inline static void
mouse_movement(GtkEventControllerMotion *self, gdouble x, gdouble y, gpointer user_data) {
  return context_handle_mouse_movement(user_data, (Point) {x, y});
}

GtkWidget *new_drawable_area(Context *context) {
  GtkWidget *area = gtk_drawing_area_new();
  context->drawing_area = area;

  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA (area), (GtkDrawingAreaDrawFunc) do_drawing, context, NULL);
  gtk_widget_set_size_request(area, 1100, 0); // todo
  gtk_widget_set_halign(area, GTK_ALIGN_FILL);
  gtk_widget_set_valign(area, GTK_ALIGN_FILL);

  // left click
  GtkGesture *gesture_click = gtk_gesture_click_new();
  gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture_click), 1);
  gtk_widget_add_controller(area, GTK_EVENT_CONTROLLER (gesture_click));
  g_signal_connect(gesture_click, "pressed", G_CALLBACK(left_clicked), context);


  // right click
  GtkGesture *gesture_right_click = gtk_gesture_click_new();
  gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture_right_click), 3);
  gtk_widget_add_controller(area, GTK_EVENT_CONTROLLER (gesture_right_click));
  g_signal_connect(gesture_right_click, "pressed", G_CALLBACK(right_clicked), context);

  // long right click
  GtkGesture *gesture_long_press = gtk_gesture_long_press_new();
  gtk_gesture_long_press_set_delay_factor(GTK_GESTURE_LONG_PRESS(gesture_long_press), .5);
  gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(gesture_long_press), 3);
  gtk_widget_add_controller(area, GTK_EVENT_CONTROLLER (gesture_long_press));
  g_signal_connect(gesture_long_press, "pressed", G_CALLBACK(long_right_click), context);

  // mouse movement
  GtkEventController *controller = gtk_event_controller_motion_new();
  gtk_widget_add_controller(area, GTK_EVENT_CONTROLLER(controller));
  g_signal_connect(controller, "motion", G_CALLBACK(mouse_movement), context);

  return area;
}
