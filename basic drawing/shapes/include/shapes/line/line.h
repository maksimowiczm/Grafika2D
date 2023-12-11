#pragma once

#include "shapes/shape/shape.h"

Shape *shapes_new_line(Point from, Point to);

double shapes_line_length(Shape line);

double shapes_line_distance_to_point(Shape line, Point point);
