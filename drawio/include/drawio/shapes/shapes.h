#pragma once

#include "stdbool.h"
#include "shapes/shape/shape.h"
#include "cairo/cairo.h"

typedef struct drawable_shape DrawableShape;
typedef struct drawable_shape_header DrawableShapeHeader;

#define COLOR(r, g, b) u_int8_t r, u_int8_t g, u_int8_t b
#define DRAW_METHOD(method_name) void (*method_name)(DrawableShape, cairo_t*, cairo_bool_t, COLOR(red, green, blue))


struct drawable_shape_header {
  bool isDrawn;
  bool shouldBeDrawn;

  DRAW_METHOD(draw_method);
};


struct drawable_shape {
  DrawableShapeHeader header;
  Shape *shape;
};
