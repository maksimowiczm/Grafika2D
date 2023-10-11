#include "shapes/point/point.h"

#include "math.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

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

void shapes_point_parse_from_string_to_reference(const char *input, Point **point) {
  char *end;
  double x = strtod(input, &end);
  if (input == end || strlen(end) == 0) {
    *point = NULL;
    return;
  }
  double y = strtod(end + 1, NULL);

  (*point)->x = x;
  (*point)->y = y;
}

inline double shapes_point_distance_to_point(Shape shape, Point point) {
  return shapes_point_distance_between_points(shape.points[0], point);
}

Point shapes_point_parse_from_string(const char *input) {
  Point result, *ptr = &result;
  shapes_point_parse_from_string_to_reference(input, &ptr);
  if (ptr == NULL) {
    return (Point) {-1, -1};
  }
  return result;
}

char *shapes_point_to_string(Point point) {
  char *str = malloc(sizeof(char) * 100);
  sprintf(str, "%lf,%lf", point.x, point.y);
  return str;
}
