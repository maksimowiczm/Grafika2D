#include "drawing_app/context.h"

#include "drawio/draw.h"
#include "drawing_app/state/no_action_state.h"

inline bool context_handle_left_click(Context *context, Point mouse) {
  return (*context->state)->handle_left_click(context, mouse);
}

inline bool context_handle_right_click(Context *context, Point mouse) {
  return (*context->state)->handle_right_click(context, mouse);
}

inline bool context_handle_right_click_long(Context *context, Point mouse) {
  return (*context->state)->handle_right_click_long(context, mouse);
}

inline void context_handle_mouse_movement(Context *context, Point mouse) {
  return (*context->state)->handle_mouse_movement(context, mouse);
}

Context *context_new(size_t shapes_max_count, size_t buffer_max_size) {
  Context *context = malloc(sizeof(*context));

  context->state = malloc(sizeof(State *));
  (*context->state) = no_action_state_get();

  context->buffer.buffer_size = buffer_max_size;
  context->buffer.buffer_current_size = 0;
  context->buffer.buffer = malloc(sizeof(Point) * buffer_max_size);

  context->currentType = Line;
  context->shapes_length = shapes_max_count;
  context->shapes = malloc(sizeof(DrawableShape *) * context->shapes_length);
  memset(context->shapes, 0, sizeof(DrawableShape *) * context->shapes_length);

  return context;
}

void context_free(Context *context, bool free_self) {
  for (int i = 0; i < context->shapes_length; i++) {
    DrawableShape *shape = context->shapes[i];
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

  if (*context->state != NULL) {
    free(*context->state);
  }
  free(context->buffer.buffer);
  free(context->state);
  free(context->shapes);
  free(context);
}

void context_state_change(Context *context, enum StateEnum newState) {
  //todo
}

void context_draw(Context *context) {
  // todo
}

void context_clear(Context *context) {
  // todo
}

void context_set_shape(Context *context, enum ShapeType type) {
  // todo
}