#pragma once

#include "shapes/shape/shape.h"

typedef struct {
  size_t length;
  Shape **lines;
} ShapeLines;

__attribute__((__deprecated__("Should be used only if shape is stored as continuous points")))
ShapeLines *shapes_get_lines(Shape *shape);

void shapes_lines_free(ShapeLines *lines, bool free_self);
