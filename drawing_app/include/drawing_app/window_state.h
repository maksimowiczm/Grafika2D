#pragma once

#include "drawio/shapes/shapes.h"
#include "gtk/gtk.h"


enum DrawingAction {
//  NoAction,
//  Drawing,
//  MovingPoint,
//  MovingShape,
  ActionCount,
};

typedef struct {
  size_t buffer_size;
  size_t buffer_current_size;
  Point *buffer;
} PointBuffer;

typedef struct {
  // lock some functionalities if user is using mouse for drawing
  enum DrawingAction action;

  // point which will be moved if action is MovingPoint
  Point *moving_point;

  // shape which will be moving id action is MovingShape
  DrawableShape **moving_shape;
  Point previous_moving_shape_position;

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

void state_moving_point_set(WindowState *state, Point *point);

void state_moving_point_move(WindowState *state, Point where);

void state_shape_choose(WindowState *state, enum ShapeType type);

void state_moving_shape_set(WindowState *state, DrawableShape *shape, Point starting_point);

void state_moving_shape_move(WindowState *state, double x, double y);

void state_redraw(WindowState *state);

void state_buffer_clear(WindowState *state);

void state_buffer_add(WindowState *state, Point point);

bool state_handle_left_click(WindowState *state, Point mouse);

bool state_handle_right_click(WindowState *state, Point mouse);

bool state_handle_right_click_long(WindowState *state, Point mouse);

void state_handle_mouse_movement(WindowState *state, Point mouse);