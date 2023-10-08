#pragma once

#include "drawio/shapes/shapes.h"


typedef struct {
  size_t shapes_length;
  DrawableShape **shapes;
} WindowState;
