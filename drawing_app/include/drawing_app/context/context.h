#pragma once

#include "gtk/gtk.h"
#include "shapes/shapes.h"
#include "stdbool.h"

typedef struct state State;
typedef struct context Context;

enum StateEnum {
  NoAction,
  Drawing,
  MovingPoint,
  MovingShape,
  StateCount,
};


struct state {
  bool (*handle_left_click)(Context *, Point);

  bool (*handle_right_click)(Context *, Point);

  bool (*handle_right_click_long)(Context *, Point);

  void (*handle_mouse_movement)(Context *, Point);
};


struct context {
  GtkWidget* drawing_area;

  State **state;
};