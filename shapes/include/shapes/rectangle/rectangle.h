#pragma once

#include "shapes/shape/shape.h"

Shape *shapes_new_rectangle(Point from, Point to);

double shapes_rectangle_distance_to_point(Shape rectangle, Point point);