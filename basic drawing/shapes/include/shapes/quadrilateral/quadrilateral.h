#pragma once

#include "shapes/shape/shape.h"

Shape *shapes_new_quadrilateral(Point *points);

double shapes_quadrilateral_distance_to_point(Shape rectangle, Point point);