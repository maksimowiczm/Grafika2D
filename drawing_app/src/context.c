#include "drawing_app/context.h"

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

Context *context_new() {
  Context *context = malloc(sizeof(*context));

  return context;
}

void context_free(Context *context, bool free_self) {
  if (free_self) {
    free(context);
  }
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
