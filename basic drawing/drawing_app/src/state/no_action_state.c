#include "drawing_app/state/no_action_state.h"
#include "drawio/cairo.h"
#include "drawing_app/context/internal_context.h"
#include "drawing_app/context.h"

State *no_action_state_get() {
  State *state = malloc(sizeof(*state));
  state->handle_left_click = no_action_state_handle_left_click;
  state->handle_right_click = no_action_state_handle_right_click;
  state->handle_right_click_long = no_action_state_handle_right_click_long;
  state->handle_mouse_movement = no_action_state_handle_mouse_movement;
  state->handle_draw_button_click = no_action_shape_state_handle_draw_button_click;
  state->draw = no_action_state_draw;
  return state;
}

bool no_action_state_handle_left_click(Context *context, Point mouse) {
  internal_context_buffer_add(context, mouse);
  internal_context_hide_user_inputs(context);
  internal_context_state_change(context, Drawing);
  return TRUE;
}

bool no_action_state_handle_right_click(Context *context, Point mouse) {
  DrawableShape *shape = internal_context_shapes_closest_to_point(context, mouse);
  if (shape == NULL) {
    return TRUE;
  }

  internal_context_buffer_clear(context);

  Point *closest = shapes_shape_closest_point(shape->shape, mouse);
  context->moving_point = closest;

  internal_context_state_change(context, MovingPoint);

  return TRUE;
}

bool no_action_state_handle_right_click_long(Context *context, Point mouse) {
  DrawableShape *shape = internal_context_shapes_closest_to_point(context, mouse);
  if (shape == NULL) {
    return TRUE;
  }

  *context->moving_shape = shape;

  internal_context_load_shape_to_user_input(context, shape->shape);

  internal_context_state_change(context, MovingShape);
  return TRUE;
}

inline void no_action_state_handle_mouse_movement(Context *context, Point mouse) {
  return;
}

void no_action_state_draw(Context *context, cairo_t *cr) {
  // draw shapes
  for (int i = 0; i < context->shapes_length; i++) {
    DrawableShape *shape = context->shapes[i];
    if (shape == NULL) {
      continue;
    }
    bool shouldBeDrawn = shape->header.shouldBeDrawn;
    if (shouldBeDrawn) {
      shape->header.draw_method(*shape, cr, true, BLACK);
      shape->header.isDrawn = true;
    }
  }

  // mark buffer
  drawio_points_mark(cr, context->buffer.buffer, context->buffer.buffer_current_size, RED);

  // mark shapes
  for (int i = 0; i < context->shapes_length; i++) {
    if (context->shapes[i] == NULL) {
      continue;
    }
    Point *points = context->shapes[i]->shape->points;
    size_t length = context->shapes[i]->shape->points_length;
    drawio_points_mark(cr, points, length, GREEN);
  }
}

static gboolean
handle_draw_error(Context *context) {
  internal_context_buffer_clear(context);
  return TRUE;
}

gboolean no_action_shape_state_handle_draw_button_click(Context *context) {
  for (int i = 0; i < shapes_point_count_to_create(context->currentType); i++) {
    GtkWidget *container = context->user_input.inputs[i];
    GtkWidget *entry = gtk_widget_get_last_child(container);
    const char *buffer = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(entry)));

    // on error
    if (buffer == NULL) {
      return handle_draw_error(context);
    }

    Point point = shapes_point_parse_from_string(buffer);
    if (point.x == -1 && point.y == -1) {
      return handle_draw_error(context);
    }

    internal_context_buffer_add(context, point);
  }

  internal_context_shapes_add(context);
  context_redraw(context);
  return TRUE;
}
