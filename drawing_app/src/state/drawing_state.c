#include "drawing_app/state/drawing_state.h"

#include "drawing_app/context/internal_context.h"
#include "drawing_app/context.h"
#include "drawio/cairo.h"
#include "drawing_app/state/no_action_state.h"


State *drawing_state_get() {
  State *state = malloc(sizeof(*state));
  state->handle_left_click = drawing_state_handle_left_click;
  state->handle_right_click = drawing_state_handle_right_click;
  state->handle_right_click_long = drawing_state_handle_right_click_long;
  state->handle_mouse_movement = drawing_state_handle_mouse_movement;
  state->draw = drawing_state_draw;
  return state;
}

bool drawing_state_handle_left_click(Context *context, Point mouse) {
  PointBuffer *buffer = &context->buffer;

  // add points to state buffer
  if (buffer->buffer_current_size + 1 > buffer->buffer_size) {
    internal_context_buffer_clear(context);
    context_state_change(context, NoAction);
    return TRUE;
  }
  internal_context_buffer_add(context, mouse);

  // handle shape creation
  if (context->buffer.buffer_current_size >= shapes_point_count_to_create(context->currentType)) {
    context_shapes_add(context);
    context_state_change(context, NoAction);
  }

  context_redraw(context);

  return TRUE;
}

inline bool drawing_state_handle_right_click(Context *context, Point mouse) {
  return TRUE;
}

inline bool drawing_state_handle_right_click_long(Context *context, Point mouse) {
  return TRUE;
}

inline void drawing_state_handle_mouse_movement(Context *context, Point mouse) {
  return;
}


inline void drawing_state_draw(Context *context, cairo_t *cr) {
  return no_action_state_draw(context, cr);
}
