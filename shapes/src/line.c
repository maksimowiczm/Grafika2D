#include "shapes/line/line.h"

#include "stdlib.h"

Shape *shapes_new_line(Point from, Point to) {
  Shape *line = malloc(sizeof(*line));

  line->points_length = 2;

  line->points = malloc(sizeof(Point) * 2);
  line->points[0] = from;
  line->points[1] = to;

  line->header.type = Line;

  return line;
}