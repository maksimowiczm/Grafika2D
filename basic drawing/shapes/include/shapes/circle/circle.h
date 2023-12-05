#pragma once

#include "shapes/shape/shape.h"

Shape *shapes_new_circle(Point center, Point edge);

double shapes_circle_radius(Shape circle);

double shapes_circle_distance_to_point(Shape circle, Point point);

Point shapes_circle_get_center(Shape circle);