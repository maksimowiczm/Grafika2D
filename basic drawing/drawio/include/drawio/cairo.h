#pragma once

#include "cairo/cairo.h"
#include "shapes/point/point.h"


#define COLOR(r, g, b) u_int8_t r, u_int8_t g, u_int8_t b

#define RED 255, 0, 0
#define GREEN 0, 255, 0
#define BLUE 0, 0, 255
#define BLACK 0, 0, 0


void drawio_line_draw(cairo_t *cairo, Point from, Point to, cairo_bool_t stroke);

void drawio_circle_draw(cairo_t *cairo, Point center, double radius, cairo_bool_t stroke);

void drawio_points_mark(cairo_t *cairo, Point *points, size_t count, u_int8_t r, u_int8_t g, u_int8_t b);
