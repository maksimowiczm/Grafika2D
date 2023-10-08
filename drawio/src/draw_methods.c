#include "drawio/draw_methods.h"


#include "shapes/shapeLines.h"
#include "drawio/cairo.h"


void drawio_loop_method(DrawableShape shape, cairo_t *cr, cairo_bool_t stroke) {
  ShapeLines *shapeLines = shapes_get_lines(shape.shape);
  Shape *lines = *shapeLines->lines;

  for (int i = 0; i < shapeLines->length; i++) {
    Point from = {lines[i].points[0].x, lines[i].points[0].y};
    Point to = {lines[i].points[0].x, lines[i].points[0].y};
    drawio_line_draw(cr, from, to, false);
  }

  if (stroke) {
    cairo_stroke(cr);
  }

  shapes_lines_free(shapeLines, true);
}
