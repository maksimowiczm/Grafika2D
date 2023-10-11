#pragma once

#include "context.h"

void internal_context_buffer_clear(Context *context);

void internal_context_buffer_add(Context *context, Point point);

void internal_context_shapes_clear(Context *context);

void internal_context_initialize_user_inputs(Context *context, size_t length);

void internal_context_free_user_inputs(Context *context);

void internal_context_hide_user_inputs(Context *context);

void internal_context_show_user_inputs(Context *context);

void internal_context_load_shape_to_user_input(Context *context, DrawableShape *shape);
