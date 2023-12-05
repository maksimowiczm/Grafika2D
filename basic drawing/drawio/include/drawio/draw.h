#pragma once

#include "shapes/shapes.h"
#include "cairo/cairo.h"


DrawableShape *drawio_new_drawableShape(Shape *shape, DRAW_METHOD(draw_method));

void drawio_drawableShape_free(DrawableShape *shape, bool free_self);
