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
  MovingShape,
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
};


struct context {
  GtkWidget *drawing_area;

  State **state;

  // store points in buffer
  PointBuffer buffer;

  // which shape is being drawn now
  enum ShapeType currentType;

  // array of shapes
  size_t shapes_length;
  DrawableShape **shapes;
};
