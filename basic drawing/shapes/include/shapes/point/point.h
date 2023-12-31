#pragma once

#include "shapes/shape/shape.h"

Shape *shapes_new_point(Point from);

double shapes_point_distance_to_point(Shape shape, Point point);

double shapes_point_distance_between_points(Point from, Point to);

void shapes_point_parse_from_string_to_reference(const char *input, Point **point);

Point shapes_point_parse_from_string(const char *input);

char *shapes_point_to_string(Point point);

char *shapes_point_to_string_int(Point point);
