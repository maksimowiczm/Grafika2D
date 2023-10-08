#pragma once

#include "cairo/cairo.h"
#include "shapes/point/point.h"

void drawio_line_draw(cairo_t *cairo, Point from, Point to, cairo_bool_t stroke);

void drawio_circle_draw(cairo_t *cairo, Point center, double radius, cairo_bool_t stroke);
