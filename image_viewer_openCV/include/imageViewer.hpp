#pragma once

#include "cinttypes"
#include "vector"

class ImageViewer {
 public:
  enum ImageType {
    PPM,
    OPEN_CV,
  };

  static bool load_image(const char *file_path, enum ImageType type);
};

