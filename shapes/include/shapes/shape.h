#pragma once

#include <sys/types.h>
#include <stdbool.h>

enum ShapeType {
  NoShape,
  Line,
  ShapesCount
};


typedef struct {
  double x;
  double y;
} Point;


typedef struct {
  enum ShapeType type;

} ShapeHeader;


typedef struct {
  ShapeHeader header;

  size_t points_length;
  Point *points;
} Shape;

typedef struct {
  size_t length;
  Shape **lines;
} ShapeLines;


Shape *shapes_new_shape(enum ShapeType type, Point *points);

void shapes_shape_free(Shape *shape, bool free_self);

ShapeLines *shapes_get_lines(Shape *shape);

void shapes_lines_free(ShapeLines *lines, bool free_self);
