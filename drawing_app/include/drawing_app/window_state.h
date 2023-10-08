#pragma once

#include "drawio/shapes/shapes.h"


typedef struct {
  size_t shapes_length;
  DrawableShape **shapes;
} WindowState;


void state_initialize(WindowState *state, size_t shapes_count);

void state_free(WindowState *state, bool free_self);

ssize_t state_add_shape(WindowState *state, DrawableShape *shape);

void state_clear_shapes(WindowState *state);
