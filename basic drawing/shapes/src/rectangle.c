#include "shapes/rectangle/rectangle.h"

#include "stdlib.h"
#include "shapes/quadrilateral/quadrilateral.h"

static void
set_header(ShapeHeader *header) {
  header->type = Rectangle;
  header->distance = shapes_rectangle_distance_to_point;
}

static void
set_points(Shape *rectangle, Point from, Point to) {
  rectangle->points_length = 2;
  rectangle->points = malloc(sizeof(Point) * 2);
  rectangle->points[0] = from;
  rectangle->points[1] = to;
}

Shape *shapes_new_rectangle(Point from, Point to) {
  Shape *rectangle = malloc(sizeof(*rectangle));

  set_header(&rectangle->header);
  set_points(rectangle, from, to);

  return rectangle;
}

double shapes_rectangle_distance_to_point(Shape rectangle, Point point) {
  /**
   *   from  --- first
   *    |          |
   *    |          |
   *  second ---  to
   */

  Shape *rectangle_as_quadrilateral = shapes_rectangle_to_quadrilateral(rectangle);
  double distance = shapes_quadrilateral_distance_to_point(*rectangle_as_quadrilateral, point);

  shapes_shape_free(rectangle_as_quadrilateral, true);

  return distance;
}

Shape *shapes_rectangle_to_quadrilateral(Shape rectangle) {
  // broooo 💀💀💀💀💀
  Point *points = malloc(sizeof(Point) * 4);
  Point from = rectangle.points[0];
  points[0] = from;
  Point to = rectangle.points[1];
  points[2] = to;
  Point first = {to.x, from.y};
  points[1] = first;
  Point second = {from.x, to.y};
  points[3] = second;

  Shape *quadrilateral = shapes_new_quadrilateral(points);
  free(points);
  return quadrilateral;
}
