#pragma once

#include "drawing_app/context/context.h"

Context *context_new(size_t shapes_max_count, size_t buffer_max_size);

void context_free(Context *context, bool free_self);

bool context_handle_left_click(Context *context, Point mouse);

bool context_handle_right_click(Context *context, Point mouse);

bool context_handle_right_click_long(Context *context, Point mouse);

void context_handle_mouse_movement(Context *context, Point mouse);

void context_state_change(Context *context, enum StateEnum newState);

void context_draw(Context *context, cairo_t *cr);

void context_redraw(Context *context);

void context_set_shape(Context *context, enum ShapeType type);

void context_shapes_add(Context *context);

void context_clear_all(Context *context);
