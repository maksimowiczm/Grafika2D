#pragma once

#include "shapes/shape/shape.h"

typedef struct {
  size_t length;
  Shape **lines;
} ShapeLines;


ShapeLines *shapes_get_lines(Shape *shape);

void shapes_lines_free(ShapeLines *lines, bool free_self);
