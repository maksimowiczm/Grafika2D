#include "shapes/point/point.h"

#include "math.h"
#include "stdio.h"
#include "stdlib.h"


static void
set_header(Shape *point) {
  point->header.type = PointAsShape;
  point->header.distance = shapes_point_distance_to_point;
}

static void
set_points(Shape *shape, Point point) {
  shape->points_length = 1;
  shape->points = malloc(sizeof(Point));
  shape->points[0] = point;
}

Shape *shapes_new_point(Point point) {
  Shape *shape = malloc(sizeof(*shape));

  set_header(shape);
  set_points(shape, point);

  return shape;
}

inline double shapes_point_distance_between_points(Point from, Point to) {
  return sqrt((from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y));
}

Point shapes_point_parse_from_string(const char *input) {
  double x = 0, y = 0;

  sscanf(input, "%lf,%lf", &x, &y); // todo safe parsing

  return (Point) {x, y};
}

inline double shapes_point_distance_to_point(Shape shape, Point point) {
  return shapes_point_distance_between_points(shape.points[0], point);
}
