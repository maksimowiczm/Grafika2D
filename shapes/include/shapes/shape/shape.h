#pragma once

#include "shapes/shapes.h"

#include <sys/types.h>
#include <stdbool.h>

Shape *shapes_new_shape(enum ShapeType type, Point *points);

void shapes_shape_free(Shape *shape, bool free_self);

double shapes_shape_distance(Shape shape, Point point);