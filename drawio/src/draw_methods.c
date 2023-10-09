#include "drawio/draw_methods.h"


#include "shapes/shapeLines.h"
#include "drawio/cairo.h"
#include "shapes/circle/circle.h"


void drawio_loop_method(DrawableShape shape, cairo_t *cr, cairo_bool_t stroke) {
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

void drawio_circle_method(DrawableShape drawableShape, cairo_t *cr, cairo_bool_t stroke) {
  Shape *circle = drawableShape.shape;
  Point center = shapes_circle_get_center(*circle);
  double radius = shapes_circle_radius(*circle);
  drawio_circle_draw(cr, center, radius, stroke);
}

void (*drawio_get_draw_method(enum ShapeType type))(DrawableShape, cairo_t *, cairo_bool_t) {
  switch (type) {
    case PointAsShape:
      return NULL;
    case Line:
    case Quadrilateral:
      return drawio_loop_method;
    case Circle:
      return drawio_circle_method;
    case NoShape:
    case ShapesCount:
      return NULL;
  }
}