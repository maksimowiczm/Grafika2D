#pragma once

#include "opencv2/core/mat.hpp"

class ImageLoader {
 public:
  enum LoaderMethod {
    PPM,
    OPEN_CV,
  };

  static cv::Mat load_image(const char *file_path, enum LoaderMethod type);
};
