#pragma once

#include "shapes/shapes.h"
#include "stdbool.h"

typedef struct state State;
typedef struct context Context;


struct state {
  bool (*handle_left_click)(Context *, Point);

  bool (*handle_right_click)(Context *, Point);

  bool (*handle_right_click_long)(Context *, Point);

  void (*handle_mouse_movement)(Context *, Point);
};


struct context {
  State **state;
};