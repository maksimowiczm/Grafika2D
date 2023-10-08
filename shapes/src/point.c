#include "shapes/point.h"

#include "math.h"
#include "stdio.h"
#include "stdlib.h"


Shape *shapes_new_point(Point point) {
  Shape *shape = malloc(sizeof(*shape));

  shape->points_length = 1;

  shape->points = malloc(sizeof(Point));
  shape->points[0] = point;

  shape->header.type = PointAsShape;

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

