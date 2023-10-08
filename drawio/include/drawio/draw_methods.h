#pragma once

#include "shapes/shapes.h"


void drawio_loop_method(DrawableShape shape, cairo_t *cr, cairo_bool_t stroke);

void drawio_circle_method(DrawableShape circle, cairo_t *cr, cairo_bool_t stroke);

void (*drawio_get_draw_method(enum ShapeType type))(DrawableShape, cairo_t *, cairo_bool_t);