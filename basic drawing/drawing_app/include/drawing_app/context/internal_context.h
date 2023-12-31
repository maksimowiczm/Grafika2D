#pragma once

#include "context.h"

void internal_context_buffer_clear(Context *context);

void internal_context_buffer_add(Context *context, Point point);

void internal_context_shapes_clear(Context *context);

void internal_context_initialize_user_inputs(Context *context, size_t length);

void internal_context_free_user_inputs(Context *context);

void internal_context_hide_user_inputs(Context *context);

void internal_context_show_user_inputs(Context *context);

void internal_context_load_shape_to_user_input(Context *context, Shape *shape);

DrawableShape *internal_context_shapes_closest_to_point(Context *context, Point point);

void internal_context_shapes_add(Context *context);

void internal_context_state_change(Context *context, enum StateEnum newType);

gboolean internal_context_apply_user_inputs(Context *context);

void internal_context_replace_shapes(Context *context, DrawableShape **shapes, size_t shapes_max_length);
