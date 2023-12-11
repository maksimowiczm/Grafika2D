#pragma once

#include "stdbool.h"

enum ImageType {
  PPM
};

typedef struct {
  enum ImageType imageType;
  void *image;
} Context;

bool context_load_image(Context *context, const char *file_path, enum ImageType type);