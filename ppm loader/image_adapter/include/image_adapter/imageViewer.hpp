#pragma once

#include "cinttypes"
#include "vector"
#include "imageLoader.hpp"

class ImageViewer {
 public:
  static bool load_image(const char *file_path, enum ImageLoader::LoaderMethod type);
};
