#include "drawing_app/state/no_action_state.h"

State *no_action_state_get() {
  State *state = malloc(sizeof(*state));
  state->handle_left_click = no_action_state_handle_left_click;
  state->handle_right_click = no_action_state_handle_right_click;
  state->handle_right_click_long = no_action_state_handle_right_click_long;
  state->handle_mouse_movement = no_action_state_handle_mouse_movement;
  return state;
}

// todo
bool no_action_state_handle_left_click(Context *context, Point mouse) {
  return 0;
}

bool no_action_state_handle_right_click(Context *context, Point mouse) {
  return 0;
}

bool no_action_state_handle_right_click_long(Context *context, Point mouse) {
  return 0;
}

void no_action_state_handle_mouse_movement(Context *context, Point mouse) {

}
