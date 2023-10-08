#include "shapes/shapeLines.h"

#include "stdlib.h"
#include "shapes/line/line.h"

ShapeLines *shapes_get_lines(Shape *shape) {
  size_t length = shape->points_length;
  if (length < 3) {
    return NULL;
  }

  ShapeLines *shapeLines = malloc(sizeof(*shapeLines));
  shapeLines->length = length;
  shapeLines->lines = malloc(sizeof(Shape *) * length);

  // as many lines as points
  Point *points = shape->points;
  for (int i = 1; i < length; i++) {
    Shape *line = shapes_new_line(points[i - 1], points[i]);
    shapeLines->lines[i] = line;
  }

  shapeLines->lines[0] = shapes_new_line(points[0], points[length - 1]);;

  return shapeLines;
}

void shapes_lines_free(ShapeLines *lines, bool free_self) {
  for (int i = 0; i < lines->length; i++) {
    shapes_shape_free(lines->lines[i], true);
  }

  free(lines->lines);
  if (free_self) {
    free(lines);
  }
}
