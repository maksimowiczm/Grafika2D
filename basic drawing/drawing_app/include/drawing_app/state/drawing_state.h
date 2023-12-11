#pragma once

#include "drawing_app/context/context.h"


State *drawing_state_get();

bool drawing_state_handle_left_click(Context *context, Point mouse);

bool drawing_state_handle_right_click(Context *context, Point mouse);

bool drawing_state_handle_right_click_long(Context *context, Point mouse);

void drawing_state_handle_mouse_movement(Context *context, Point mouse);

void drawing_state_draw(Context *context, cairo_t *cr);

gboolean drawing_state_handle_draw_button_click(Context *context);
