#pragma once

#include "shapes/shape/shape.h"

Shape *shapes_new_point(Point from);

double shapes_point_distance_to_point(Shape shape, Point point);

double shapes_point_distance_between_points(Point from, Point to);

Point shapes_point_parse_from_string(const char *input);