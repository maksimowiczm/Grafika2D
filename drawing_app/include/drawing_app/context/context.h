#pragma once

#include "gtk/gtk.h"
#include "shapes/shapes.h"
#include "stdbool.h"
#include "drawio/shapes/shapes.h"


typedef struct state State;
typedef struct context Context;


enum StateEnum {
  NoAction,
  Drawing,
  MovingPoint,
  MovingShapeMouse,
  StateCount,
};


typedef struct {
  size_t buffer_size;
  size_t buffer_current_size;
  Point *buffer;
} PointBuffer;


struct state {
  bool (*handle_left_click)(Context *context, Point mouse);

  bool (*handle_right_click)(Context *context, Point mouse);

  bool (*handle_right_click_long)(Context *context, Point mouse);

  void (*handle_mouse_movement)(Context *context, Point mouse);

  gboolean (*handle_draw_button_click)(Context *context);

  void (*draw)(Context *context, cairo_t *cr);
};

typedef struct {
  size_t inputs_length;
  GtkWidget **inputs;
  GtkWidget *draw_button;
} UserInput;


struct context {
  GtkWidget *drawing_area;
  UserInput user_input;

  // point which will be moved if action is MovingPoint
  Point *moving_point;

  // shape which will be moving id action is MovingShapeMouse
  DrawableShape **moving_shape;
  Point previous_moving_shape_position;

  State **state;

  // store points in buffer
  PointBuffer buffer;

  // which shape is being drawn now
  enum ShapeType currentType;

  // array of shapes
  size_t shapes_length;
  DrawableShape **shapes;
};
