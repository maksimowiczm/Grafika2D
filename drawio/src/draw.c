#include "drawio/draw.h"

static void
set_header(DrawableShapeHeader *header, DRAW_METHOD(draw_method)) {
  header->isDrawn = false;
  header->shouldBeDrawn = true;
  header->draw_method = draw_method;
}

DrawableShape *drawio_new_drawableShape(Shape *shape, DRAW_METHOD(draw_method)) {
  DrawableShape *drawableShape = malloc(sizeof(*drawableShape));

  drawableShape->shape = shape;
  set_header(&drawableShape->header, draw_method);

  return drawableShape;
}

void drawio_drawableShape_free(DrawableShape *shape, bool free_self) {
  if (free_self) {
    free(shape);
  }
}
