#include "shapes/line/line.h"

#include "stdlib.h"
#include "math.h"
#include "shapes/point/point.h"

static void
set_header(Shape *line) {
  line->header.type = Line;
  line->header.distance = shapes_line_distance_to_point;
}

static void
set_points(Shape *line, Point from, Point to) {
  line->points_length = 2;
  line->points = malloc(sizeof(Point) * 2);
  line->points[0] = from;
  line->points[1] = to;
}

Shape *shapes_new_line(Point from, Point to) {
  Shape *line = malloc(sizeof(*line));

  set_header(line);
  set_points(line, from, to);

  return line;
}

inline double shapes_line_length(Shape line) {
  return shapes_point_distance_between_points(line.points[0], line.points[1]);
}

double shapes_line_distance_to_point(Shape line, Point point) {
  Point start = line.points[0];
  Point end = line.points[1];

  // FROGOR
  if ((point.x < start.x && point.y < end.x) || (point.x > start.x && point.x > end.x)) {
    double to_start = shapes_point_distance_between_points(point, start);
    double to_end = shapes_point_distance_between_points(point, end);
    return to_start < to_end ? to_start : to_end;
  }

  return
      fabs((end.x - start.x) * (start.y - point.y) -
           (start.x - point.x) * (end.y - start.y))
      / shapes_line_length(line);
}