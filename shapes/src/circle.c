#include "shapes/circle/circle.h"

#include "shapes/point/point.h"
#include "math.h"


static void
set_header(ShapeHeader *header) {
  header->type = Circle;
  header->distance = shapes_circle_distance_to_point;
}

static void
set_points(Shape *circle, Point center, Point edge) {
  circle->points_length = 2;
  circle->points = malloc(sizeof(Point) * 2);
  circle->points[0] = center;
  circle->points[1] = edge;
}

Shape *shapes_new_circle(Point center, Point edge) {
  Shape *circle = malloc(sizeof(*circle));

  set_header(&circle->header);
  set_points(circle, center, edge);

  return circle;
}

inline double shapes_circle_radius(Shape circle) {
  return shapes_point_distance_between_points(circle.points[0], circle.points[1]);
}

inline double shapes_circle_distance_to_point(Shape circle, Point point) {
  return fabs(shapes_point_distance_between_points(circle.points[0], point) - shapes_circle_radius(circle));
}

inline Point shapes_circle_get_center(Shape circle) {
  return circle.points[0];
}
