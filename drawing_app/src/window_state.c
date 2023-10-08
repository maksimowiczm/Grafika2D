#include <string.h>
#include "drawing_app/window_state.h"


#include "drawio/draw.h"
#include "drawio/draw_methods.h"


void state_initialize(WindowState *state, size_t shapes_max_count, size_t buffer_max_size) {
  state->drawing = false;
  state->currentType = Line;
  state->buffer.buffer_size = buffer_max_size;
  state->buffer.buffer_current_size = 0;
  state->buffer.buffer = malloc(sizeof(Point) * buffer_max_size);

  state->shapes_length = shapes_max_count;
  state->shapes = malloc(sizeof(DrawableShape **) * state->shapes_length);
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
  free(state->buffer.buffer);
  free(state->shapes);
  free(state);
}


static ssize_t
internal_state_add_shape(WindowState *state, DrawableShape *shape) {
  for (int i = 0; i < state->shapes_length; i++) {
    if (state->shapes[i] == NULL) {
      *(state->shapes + i) = shape;
      return i;
    }
  }

  return -1;
}


void state_add_shape(WindowState *state) {
  Shape *shape = shapes_new_shape(state->currentType, state->buffer.buffer);
  DrawableShape *drawableShape = drawio_new_drawableShape(shape, drawio_get_draw_method(state->currentType));
  internal_state_add_shape(state, drawableShape);
  state_buffer_clear(state);
}


void state_clear_shapes(WindowState *state) {
  state_free(state, false);
  memset(state->shapes, 0, sizeof(DrawableShape **) * state->shapes_length);
}


void state_buffer_clear(WindowState *state) {
  state->drawing = false;
  state->buffer.buffer_current_size = 0;
  memset(state->buffer.buffer, 0, sizeof(Point) * state->buffer.buffer_size);
}


void state_buffer_add(WindowState *state, Point point) {
  state->drawing = true;
  state->buffer.buffer[state->buffer.buffer_current_size] = point;
  state->buffer.buffer_current_size++;
}
