#pragma once

#include "drawio/shapes/shapes.h"


typedef struct {
  // lock some functionalities if user is using mouse for drawing
  bool drawing;

  // which shape is being drawn now
  enum ShapeType currentType;

  // store points in buffer
  size_t buffer_size;
  size_t buffer_current_size;
  Point *buffer;

  // array of shapes
  size_t shapes_length;
  DrawableShape **shapes;
} WindowState;


void state_initialize(WindowState *state, size_t shapes_count, size_t buffer_max_size);

void state_free(WindowState *state, bool free_self);

ssize_t state_add_shape(WindowState *state, DrawableShape *shape);

void state_clear_shapes(WindowState *state);

void state_clear_buffer(WindowState *state);
