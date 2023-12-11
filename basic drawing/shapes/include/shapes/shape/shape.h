#pragma once

#include "shapes/shapes.h"

#include <sys/types.h>
#include <stdbool.h>

Shape *shapes_new_shape(enum ShapeType type, Point *points);

size_t shapes_point_count_to_create(enum ShapeType type);

void shapes_shape_free(Shape *shape, bool free_self);

double shapes_shape_distance(Shape shape, Point point);

Point *shapes_shape_closest_point(Shape *shape, Point point);

void shapes_shape_move(Shape *shape, Vector2D vector);

double (*shapes_get_distance_method(enum ShapeType type))(Shape, Point);
