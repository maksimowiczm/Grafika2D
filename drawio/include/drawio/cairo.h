#pragma once

#include "cairo/cairo.h"
#include "shapes/point/point.h"

void drawio_line_draw(cairo_t *cairo, Point from, Point to, cairo_bool_t stroke);

void drawio_circle_draw(cairo_t *cairo, Point center, double radius, cairo_bool_t stroke);

void drawio_points_mark(cairo_t *cairo, Point *points, size_t count, u_int8_t r, u_int8_t g, u_int8_t b);
