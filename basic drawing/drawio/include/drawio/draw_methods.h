#pragma once

#include "shapes/shapes.h"
#include "drawio/cairo.h"


void drawio_loop_method(DrawableShape drawableShape, cairo_t *cr, cairo_bool_t stroke, COLOR(r, g, b));

void drawio_circle_method(DrawableShape drawableShape, cairo_t *cr, cairo_bool_t stroke, COLOR(r, g, b));

void drawio_rectangle_method(DrawableShape drawableShape, cairo_t *ct, cairo_bool_t stroke, COLOR(r, g, b));

void (*drawio_get_draw_method(enum ShapeType type))(DrawableShape, cairo_t *, cairo_bool_t, COLOR(r, g, b));
