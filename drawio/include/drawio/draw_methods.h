#pragma once

#include "shapes/shapes.h"


void drawio_loop_method(DrawableShape drawableShape, cairo_t *cr, cairo_bool_t stroke);

void drawio_circle_method(DrawableShape drawableShape, cairo_t *cr, cairo_bool_t stroke);

void drawio_rectangle_method(DrawableShape drawableShape, cairo_t *ct, cairo_bool_t stroke);

void (*drawio_get_draw_method(enum ShapeType type))(DrawableShape, cairo_t *, cairo_bool_t);