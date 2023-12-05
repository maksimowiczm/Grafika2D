#include "drawing_app/state/moving_shape_mouse_state.h"

#include "drawing_app/context.h"
#include "drawing_app/context/internal_context.h"
#include "drawing_app/state/no_action_state.h"
#include "drawio/cairo.h"

State *moving_shape_mouse_state_get() {
  State *state = malloc(sizeof(*state));
  state->handle_left_click = moving_shape_mouse_state_handle_left_click;
  state->handle_right_click = moving_shape_mouse_state_handle_right_click;
  state->handle_right_click_long = moving_shape_mouse_state_handle_right_click_long;
  state->handle_mouse_movement = moving_shape_mouse_state_handle_mouse_movement;
  state->handle_draw_button_click = mmoving_shape_mouse_state_handle_draw_button_click;
  state->draw = moving_shape_mouse_state_draw;
  return state;
}

bool moving_shape_mouse_state_handle_left_click(Context *context, Point mouse) {
  *context->moving_shape = NULL;
  internal_context_state_change(context, NoAction);
  return TRUE;
}

inline bool moving_shape_mouse_state_handle_right_click(Context *context, Point mouse) {
  internal_context_state_change(context, MovingShape);
  return FALSE;
}

inline bool moving_shape_mouse_state_handle_right_click_long(Context *context, Point mouse) {
  return FALSE;
}

void moving_shape_mouse_state_handle_mouse_movement(Context *context, Point mouse) {
  Vector2D vector = {-(context->previous_moving_shape_position.x - mouse.x),
                     -(context->previous_moving_shape_position.y - mouse.y)};
  context->previous_moving_shape_position.x = mouse.x;
  context->previous_moving_shape_position.y = mouse.y;

  Shape *shape = (*context->moving_shape)->shape;
  shapes_shape_move(shape, vector);
  context_redraw(context);
  internal_context_load_shape_to_user_input(context, shape);
}

void moving_shape_mouse_state_draw(Context *context, cairo_t *cr) {
  no_action_state_draw(context, cr);

  DrawableShape *shape = *context->moving_shape;
  shape->header.draw_method(*shape, cr, true, RED);
  drawio_points_mark(cr, shape->shape->points, shape->shape->points_length, RED);
}

inline gboolean mmoving_shape_mouse_state_handle_draw_button_click(Context *context) {
  return TRUE;
}
