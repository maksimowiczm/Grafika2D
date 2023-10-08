#include "shapes/shape.h"

#include "stdlib.h"
#include "shapes/line/line.h"
#include "shapes/rectangle/rectangle.h"


Shape *shapes_new_shape(enum ShapeType type, Point *points) {
  if (type == Line) {
    return shapes_new_line(points[0], points[1]);
  } else if (type == Rectangle) {
    return shapes_new_rectangle(points[0], points[1]);
  }

  return NULL;
}

void shapes_shape_free(Shape *shape, bool free_self) {
  free(shape->points);
  if (free_self) {
    free(shape);
  }
}
