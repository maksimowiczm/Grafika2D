#include <string.h>
#include "drawing_app/window_state.h"


#include "drawio/draw.h"


void state_initialize(WindowState *state, size_t shapes_max_count) {
  state->shapes_length = shapes_max_count;
  state->shapes = malloc(sizeof(DrawableShape **) * state->shapes_length);
  memset(state->shapes, 0, sizeof(DrawableShape **) * state->shapes_length);
}

void state_clear_shapes(WindowState *state) {
  state_free(state, false);
  memset(state->shapes, 0, sizeof(DrawableShape **) * state->shapes_length);
}

void state_free(WindowState *state, bool free_self) {
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

  if (!free_self) {
    return;
  }

  // free state
  free(state->shapes);
  free(state);
}

ssize_t state_add_shape(WindowState *state, DrawableShape *shape) {
  for (int i = 0; i < state->shapes_length; i++) {
    if (state->shapes[i] == NULL) {
      *(state->shapes + i) = shape;
      return i;
    }
  }

  return -1;
}
