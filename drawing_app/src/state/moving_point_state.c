#include "drawing_app/state/moving_point_state.h"

#include "drawing_app/context.h"
#include "drawing_app/context/internal_context.h"
#include "drawing_app/state/no_action_state.h"
#include "drawio/cairo.h"


State *moving_point_state_get() {
  State *state = malloc(sizeof(*state));
  state->handle_left_click = moving_point_state_handle_left_click;
  state->handle_right_click = moving_point_state_handle_right_click;
  state->handle_right_click_long = moving_point_state_handle_right_click_long;
  state->handle_mouse_movement = moving_point_state_handle_mouse_movement;
  state->draw = moving_point_state_draw;
  return state;
}

bool moving_point_state_handle_left_click(Context *context, Point mouse) {
  context->moving_point->x = mouse.x;
  context->moving_point->y = mouse.y;
  context_state_change(context, NoAction);
  return TRUE;
}

bool moving_point_state_handle_right_click(Context *context, Point mouse) {
  internal_context_buffer_clear(context);
  context_state_change(context, NoAction);
  return TRUE;
}

inline bool moving_point_state_handle_right_click_long(Context *context, Point mouse) {
  return no_action_state_handle_right_click_long(context, mouse);
}

inline void moving_point_state_handle_mouse_movement(Context *context, Point mouse) {
  return;
}

void moving_point_state_draw(Context *context, cairo_t *cr) {
  no_action_state_draw(context, cr);
  drawio_points_mark(cr, context->moving_point, 1, BLUE);
}
