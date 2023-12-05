#pragma once

#include "drawing_app/context/context.h"


State *no_action_state_get();

bool no_action_state_handle_left_click(Context *context, Point mouse);

bool no_action_state_handle_right_click(Context *context, Point mouse);

bool no_action_state_handle_right_click_long(Context *context, Point mouse);

void no_action_state_handle_mouse_movement(Context *context, Point mouse);

void no_action_state_draw(Context *context, cairo_t *cr);

gboolean no_action_shape_state_handle_draw_button_click(Context *context);
