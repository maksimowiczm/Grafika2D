#pragma once

class ImageWrapper {
  cv::Mat imageMat;
  void **to_free{};

 public:
  ImageWrapper(int width, int height, uint8_t *pixels, void *to_free);
  explicit ImageWrapper(cv::Mat mat);
  ~ImageWrapper();

  [[nodiscard]] const cv::Mat &GetImageMat() const;
};