#pragma once

#include "drawio/shapes/shapes.h"
#include "gtk/gtk.h"


typedef struct {
  size_t buffer_size;
  size_t buffer_current_size;
  Point *buffer;
} PointBuffer;

typedef struct {
  // lock some functionalities if user is using mouse for drawing
  bool drawing;

  // which shape is being drawn now
  enum ShapeType currentType;

  GtkWidget *drawing_area;

  // store points in buffer
  PointBuffer buffer;

  // array of shapes
  size_t shapes_length;
  DrawableShape **shapes;
} WindowState;


void state_initialize(WindowState *state, size_t shapes_count, size_t buffer_max_size);

void state_free(WindowState *state, bool free_self);

void state_shapes_add(WindowState *state);

void state_shapes_clear(WindowState *state);

DrawableShape *state_shapes_closest_shape(WindowState *state, Point point);

void state_buffer_clear(WindowState *state);

void state_buffer_add(WindowState *state, Point point);
