#pragma once

#include "stdlib.h"

typedef struct shape Shape;
typedef struct point Point;
typedef struct shape_header ShapeHeader;


enum ShapeType {
  NoShape,
  PointAsShape,
  Line,
  Quadrilateral,
  Rectangle,
  Circle,
  ShapesCount
};


struct point {
  double x;
  double y;
};


struct shape_header {
  enum ShapeType type;

  double (*distance)(Shape, Point);
};


struct shape {
  ShapeHeader header;

  size_t points_length;
  Point *points;
};
