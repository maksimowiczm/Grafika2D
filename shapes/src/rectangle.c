#include "shapes/rectangle/rectangle.h"

#include "stdlib.h"

Shape *shapes_new_rectangle(Point from, Point to) {
  Shape *rectangle = malloc(sizeof(*rectangle));

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

  rectangle->header.type = Rectangle;

  return rectangle;
}
