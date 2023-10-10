#pragma once

#include "drawing_app/context/context.h"

Context *context_new();

void context_free(Context *context, bool free_self);

bool context_handle_left_click(Context *context, Point mouse);

bool context_handle_right_click(Context *context, Point mouse);

bool context_handle_right_click_long(Context *context, Point mouse);

void context_handle_mouse_movement(Context *context, Point mouse);

void context_state_change(Context *context, enum StateEnum newState);
