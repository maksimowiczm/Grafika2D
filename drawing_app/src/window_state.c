#include <string.h>
#include "drawing_app/window_state.h"


#include "drawio/draw.h"
#include "drawio/draw_methods.h"


void state_initialize(WindowState *state, size_t shapes_max_count, size_t buffer_max_size) {
  state->drawing_area = NULL;

  state->action = NoAction;
  state->currentType = Line;
  state->buffer.buffer_size = buffer_max_size;
  state->buffer.buffer_current_size = 0;
  state->buffer.buffer = malloc(sizeof(Point) * buffer_max_size);

  state->shapes_length = shapes_max_count;
  state->shapes = malloc(sizeof(DrawableShape **) * state->shapes_length);
  memset(state->shapes, 0, sizeof(DrawableShape **) * state->shapes_length);

  state->moving_shape = malloc(sizeof(DrawableShape **));
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
  free(state->moving_shape);
  free(state);
}


static ssize_t
internal_state_add_shape(WindowState *state, DrawableShape *shape, bool override) {
  for (int i = 0; i < state->shapes_length; i++) {
    if (state->shapes[i] == NULL) {
      *(state->shapes + i) = shape;
      return i;
    }
  }

  static size_t override_index = 0;
  if (override) {
    // free memory
    DrawableShape *drawableShape = *(state->shapes + override_index);
    shapes_shape_free(drawableShape->shape, true);
    drawio_drawableShape_free(*(state->shapes + override_index), true);

    // override
    *(state->shapes + override_index) = shape;
    override_index = (override_index + 1) % state->shapes_length;
    return override_index - 1;
  }

  return -1;
}


void state_shapes_add(WindowState *state) {
  Shape *shape = shapes_new_shape(state->currentType, state->buffer.buffer);
  DrawableShape *drawableShape = drawio_new_drawableShape(shape, drawio_get_draw_method(state->currentType));
  internal_state_add_shape(state, drawableShape, true);
  state_buffer_clear(state);
}


void state_shapes_clear(WindowState *state) {
  state_free(state, false);
  memset(state->shapes, 0, sizeof(DrawableShape **) * state->shapes_length);
}


void state_buffer_clear(WindowState *state) {
  state->action = NoAction;
  state->buffer.buffer_current_size = 0;
  memset(state->buffer.buffer, 0, sizeof(Point) * state->buffer.buffer_size);
}


void state_buffer_add(WindowState *state, Point point) {
  state->action = Drawing;
  state->buffer.buffer[state->buffer.buffer_current_size] = point;
  state->buffer.buffer_current_size++;
}


DrawableShape *state_shapes_closest_shape(WindowState *state, Point point) {
  double closest = DBL_MAX;
  int index = -1;
  for (int i = 0; i < state->shapes_length; i++) {
    DrawableShape *drawable = *(state->shapes + i);
    if (drawable == NULL || !drawable->header.isDrawn) {
      continue;
    }
    Shape *shape = drawable->shape;

    double distance = shapes_shape_distance(*shape, point);
    if (distance < closest) {
      closest = distance;
      index = i;
    }
  }

  if (index != -1 && closest < 10) {
    return state->shapes[index];
  }

  return NULL;
}


void state_moving_point_set(WindowState *state, Point *point) {
  state->moving_point = point;
  state->action = MovingPoint;
}


void state_moving_point_move(WindowState *state, Point where) {
  state->moving_point->x = where.x;
  state->moving_point->y = where.y;
  state->action = NoAction;
}


void state_redraw(WindowState *state) {
  if (state->drawing_area == NULL) {
    return;
  }

  gtk_widget_queue_draw(state->drawing_area);
}

void state_shape_choose(WindowState *state, enum ShapeType type) {
  state->currentType = type;
  state_buffer_clear(state);
  state_redraw(state);
}


void state_moving_shape_set(WindowState *state, DrawableShape *shape, Point starting_point) {
  state->action = MovingShape;
  *state->moving_shape = shape;
  state->previous_moving_shape_position = starting_point;
}


void state_moving_shape_move(WindowState *state, double x, double y) {
  state->action = NoAction;
  *state->moving_shape = NULL;
}


bool state_handle_left_click(WindowState *state, Point mouse) {
  if (state->action == NoAction || state->action == Drawing) {
    PointBuffer *buffer = &state->buffer;

    // add points to state buffer
    if (buffer->buffer_current_size + 1 > buffer->buffer_size) {
      state_buffer_clear(state);
      state_redraw(state);
      return TRUE;
    }
    state_buffer_add(state, mouse);

    // handle shape creation
    if (state->buffer.buffer_current_size >= shapes_point_count_to_create(state->currentType)) {
      state_shapes_add(state);
    }

    state_redraw(state);
    return TRUE;
  } else if (state->action == MovingPoint) {
    state_moving_point_move(state, mouse);
    state_redraw(state);
    return TRUE;
  } else if (state->action == MovingShape) {
    state_moving_shape_move(state, mouse.x, mouse.y);
    state_redraw(state);
    return TRUE;
  }

  return TRUE;
}

bool state_handle_right_click(WindowState *state, Point mouse) {
  if (state->action == MovingShape) {
    return FALSE;
  }

  if (state->action == MovingPoint) {
    state_buffer_clear(state);
    state->action = NoAction;
    state_redraw(state);
    return TRUE;
  }

  DrawableShape *shape = state_shapes_closest_shape(state, mouse);
  if (shape == NULL) {
    return TRUE;
  }
  state_buffer_clear(state);

  Point *closest = shapes_shape_closest_point(shape->shape, mouse);
  state_moving_point_set(state, closest);

  state_redraw(state);

  return TRUE;
}

bool state_handle_right_click_long(WindowState *state, Point mouse) {
  if (state->action == MovingShape) {
    return FALSE;
  }

  DrawableShape *shape = state_shapes_closest_shape(state, mouse);
  if (shape == NULL) {
    return TRUE;
  }
  state_moving_shape_set(state, shape, mouse);
  state_redraw(state);

  return TRUE;
}

void state_handle_mouse_movement(WindowState *state, Point mouse) {
  if (state->action != MovingShape) {
    return;
  }

  Vector2D vector = {-(state->previous_moving_shape_position.x - mouse.x),
                     -(state->previous_moving_shape_position.y - mouse.y)};
  state->previous_moving_shape_position.x = mouse.x;
  state->previous_moving_shape_position.y = mouse.y;

  Shape *shape = (*state->moving_shape)->shape;
  shapes_shape_move(shape, vector);
  state_redraw(state);
}
