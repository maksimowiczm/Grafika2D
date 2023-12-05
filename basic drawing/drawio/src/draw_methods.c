#include "drawio/draw_methods.h"


#include "shapes/shapeLines.h"
#include "drawio/cairo.h"
#include "shapes/circle/circle.h"
#include "shapes/rectangle/rectangle.h"


void drawio_loop_method(DrawableShape shape, cairo_t *cr, cairo_bool_t stroke, COLOR(red, green, blue)) {
  cairo_set_source_rgb(cr, red, green, blue);
  ShapeLines *shapeLines = shapes_get_lines(shape.shape);

  for (int i = 0; i < shapeLines->length; i++) {
    Point *points = shapeLines->lines[i]->points;
    Point from = {points[0].x, points[0].y};
    Point to = {points[1].x, points[1].y};
    drawio_line_draw(cr, from, to, false);
  }

  if (stroke) {
    cairo_stroke(cr);
  }

  shapes_lines_free(shapeLines, true);
}

void drawio_circle_method(DrawableShape drawableShape, cairo_t *cr, cairo_bool_t stroke, COLOR(red, green, blue)) {
  cairo_set_source_rgb(cr, red, green, blue);
  Shape *circle = drawableShape.shape;
  Point center = shapes_circle_get_center(*circle);
  double radius = shapes_circle_radius(*circle);
  drawio_circle_draw(cr, center, radius, stroke);
}

void drawio_rectangle_method(DrawableShape drawableShape, cairo_t *ct, cairo_bool_t stroke, COLOR(red, green, blue)) {
  Shape *rectangle = drawableShape.shape;
  Shape *quadrilateral = shapes_rectangle_to_quadrilateral(*rectangle);
  DrawableShape quadrilateral_DRAW = {0};
  quadrilateral_DRAW.shape = quadrilateral;
  drawio_loop_method(quadrilateral_DRAW, ct, stroke, red, green, blue);
  shapes_shape_free(quadrilateral, true);
}

void (*drawio_get_draw_method(enum ShapeType type))(DrawableShape, cairo_t *, cairo_bool_t, COLOR(r, g, b)) {
  switch (type) {
    case PointAsShape:
      return NULL;
    case Line:
    case Quadrilateral:
      return drawio_loop_method;
    case Circle:
      return drawio_circle_method;
    case Rectangle:
      return drawio_rectangle_method;
    case NoShape:
    case ShapesCount:
      return NULL;
  }
}
