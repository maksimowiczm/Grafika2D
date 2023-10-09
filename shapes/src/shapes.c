#include "shapes/shape/shape.h"

#include "stdlib.h"
#include "stdint.h"
#include "float.h"
#include "shapes/line/line.h"
#include "shapes/point/point.h"
#include "shapes/circle/circle.h"
#include "shapes/quadrilateral/quadrilateral.h"
#include "shapes/rectangle/rectangle.h"


Shape *shapes_new_shape(enum ShapeType type, Point *points) {
  if (type == PointAsShape) {
    return shapes_new_point(points[0]);
  } else if (type == Line) {
    return shapes_new_line(points[0], points[1]);
  } else if (type == Quadrilateral) {
    return shapes_new_quadrilateral(points);
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
    case Circle:
    case Rectangle:
      return 2;
    case Quadrilateral:
      return 4;
    case NoShape:
    case ShapesCount:
      return SIZE_MAX;
  }
}

Point *shapes_shape_closest_point(Shape *shape, Point point) {
  double distance = DBL_MAX;
  Point *result = NULL;
  for (int i = 0; i < shape->points_length; i++) {
    Point *currentPoint = shape->points + i;
    double currentDistance = shapes_point_distance_between_points(*currentPoint, point);
    if (currentDistance < distance) {
      result = currentPoint;
      distance = currentDistance;
    }
  }

  return result;
}
