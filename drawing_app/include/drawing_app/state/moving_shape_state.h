#pragma once

#include "drawing_app/context/context.h"

State *moving_shape_state_get();

bool moving_shape_state_handle_left_click(Context *context, Point mouse);

bool moving_shape_state_handle_right_click(Context *context, Point mouse);

bool moving_shape_state_handle_right_click_long(Context *context, Point mouse);

void moving_shape_state_handle_mouse_movement(Context *context, Point mouse);

void moving_shape_state_draw(Context *context, cairo_t *cr);
