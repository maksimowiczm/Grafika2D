#include <string.h>
#include "shapes/quadrilateral/quadrilateral.h"

#include "stdlib.h"
#include "float.h"
#include "shapes/shapeLines.h"

static void
set_header(ShapeHeader *header) {
  header->type = Quadrilateral;
  header->distance = shapes_quadrilateral_distance_to_point;
}

static void
set_points(Shape *quadrilateral, Point *points) {
  quadrilateral->points_length = 4;
  quadrilateral->points = malloc(sizeof(Point) * 4);
  memcpy(quadrilateral->points, points, sizeof(Point) * 4);
}

Shape *shapes_new_quadrilateral(Point *points) {
  Shape *quadrilateral = malloc(sizeof(*quadrilateral));

  set_header(&quadrilateral->header);
  set_points(quadrilateral, points);

  return quadrilateral;
}

double shapes_quadrilateral_distance_to_point(Shape quadrilateral, Point point) {
  double distance = DBL_MAX;

  ShapeLines *lines = shapes_get_lines(&quadrilateral);

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