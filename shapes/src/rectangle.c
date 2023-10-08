#include "shapes/rectangle/rectangle.h"

#include "stdlib.h"
#include "float.h"
#include "shapes/shapeLines.h"

static void
set_header(ShapeHeader *header) {
  header->type = Rectangle;
  header->distance = shapes_rectangle_distance_to_point;
}

static void
set_points(Shape *rectangle, Point from, Point to) {
  rectangle->points_length = 4;

  /**
   *   from  --- first
   *    |          |
   *    |          |
   *  second ---  to
   */

  Point first = {to.x, from.y};
  Point second = {from.x, to.y};

  rectangle->points = malloc(sizeof(Point) * 4);
  rectangle->points[0] = from;
  rectangle->points[1] = first;
  rectangle->points[2] = to;
  rectangle->points[3] = second;
}

Shape *shapes_new_rectangle(Point from, Point to) {
  Shape *rectangle = malloc(sizeof(*rectangle));

  set_header(&rectangle->header);
  set_points(rectangle, from, to);

  return rectangle;
}

double shapes_rectangle_distance_to_point(Shape rectangle, Point point) {
  double distance = DBL_MAX;

  ShapeLines *lines = shapes_get_lines(&rectangle);

  for (int i = 0; i < lines->length; i++) {
    Shape line = *lines->lines[i];
    double current = line.header.distance(line, point);

    if (current < distance) {
      distance = current;
    }
  }

  shapes_lines_free(lines, true);

  return distance;
}