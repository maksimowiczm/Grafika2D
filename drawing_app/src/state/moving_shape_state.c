#pragma once

#include "drawing_app/state/moving_shape_state.h"

#include "drawing_app/context/context.h"
#include "drawing_app/state/moving_shape_mouse_state.h"
#include "drawing_app/context/internal_context.h"

State *moving_shape_state_get() {
  State *state = malloc(sizeof(*state));
  state->handle_left_click = moving_shape_state_handle_left_click;
  state->handle_right_click = moving_shape_state_handle_right_click;
  state->handle_right_click_long = moving_shape_state_handle_right_click_long;
  state->handle_mouse_movement = moving_shape_state_handle_movement;
  state->handle_draw_button_click = moving_shape_state_handle_draw_button_click;
  state->draw = moving_shape_state_draw;
  return state;
}

inline bool moving_shape_state_handle_left_click(Context *context, Point mouse) {
  *context->moving_shape = NULL;
  internal_context_state_change(context, NoAction);
  return FALSE;
}

bool moving_shape_state_handle_right_click(Context *context, Point mouse) {
  internal_context_state_change(context, MovingShapeMouse);
  return FALSE;
}

inline bool moving_shape_state_handle_right_click_long(Context *context, Point mouse) {
  return FALSE;
}

inline void moving_shape_state_handle_movement(Context *context, Point mouse) {
  return;
}

inline void moving_shape_state_draw(Context *context, cairo_t *cr) {
  return moving_shape_mouse_state_draw(context, cr);
}

gboolean moving_shape_state_handle_draw_button_click(Context *context) {
  return TRUE;
}
