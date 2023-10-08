#include "shapes/shape/shape.h"

#include "stdlib.h"
#include "stdint.h"
#include "shapes/line/line.h"
#include "shapes/rectangle/rectangle.h"
#include "shapes/point/point.h"
#include "shapes/circle/circle.h"


Shape *shapes_new_shape(enum ShapeType type, Point *points) {
  if (type == PointAsShape) {
    return shapes_new_point(points[0]);
  } else if (type == Line) {
    return shapes_new_line(points[0], points[1]);
  } else if (type == Rectangle) {
    return shapes_new_rectangle(points[0], points[1]);
  } else if (type == Circle) {
    return shapes_new_circle(points[0], points[1]);
  }

  return NULL;
}

void shapes_shape_free(Shape *shape, bool free_self) {
  free(shape->points);
  if (free_self) {
    free(shape);
  }
}

inline double shapes_shape_distance(Shape shape, Point point) {
  return shape.header.distance(shape, point);
}

size_t shapes_point_count_to_create(enum ShapeType type) {
  switch (type) {
    case PointAsShape:
      return 1;
    case Line:
    case Rectangle:
    case Circle:
      return 2;
    case NoShape:
    case ShapesCount:
      return SIZE_MAX;
  }
}
