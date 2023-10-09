#include "drawio/cairo.h"


#include "math.h"


void drawio_line_draw(cairo_t *cairo, Point from, Point to, cairo_bool_t stroke) {
  cairo_move_to(cairo, from.x, from.y);
  cairo_line_to(cairo, to.x, to.y);

  if (stroke) {
    cairo_stroke(cairo);
  }
}

void drawio_circle_draw(cairo_t *cairo, Point center, double radius, cairo_bool_t stroke) {
  cairo_arc(cairo, center.x, center.y, (uint) radius, 0, 2 * M_PI);

  if (stroke) {
    cairo_stroke(cairo);
  }
}

void drawio_points_mark(cairo_t *cairo, Point *points, size_t count, u_int8_t r, u_int8_t g, u_int8_t b) {
  cairo_set_source_rgb(cairo, r, g, b);

  for (int i = 0; i < count; i++) {
    Point point = points[i];
    cairo_arc(cairo, point.x, point.y, 3, 0, 2 * M_PI);
    cairo_fill(cairo);
  }
}
