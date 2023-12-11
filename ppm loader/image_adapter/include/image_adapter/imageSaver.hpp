#pragma once

#include "opencv2/core/mat.hpp"

class ImageSaver {
 public:
  static void save_jpeg(const std::string &file_path, const cv::Mat& image, int jpeg_quality);
};
