#include <opencv2/core/mat.hpp>
#include <utility>
#include "image_gui_gtk/ImageWrapper.hpp"

ImageWrapper::ImageWrapper(int width, int height, uint8_t *pixels, void *to_free) {
  this->to_free = new void *;
  *this->to_free = to_free;
  imageMat = cv::Mat{height, width, CV_8UC3, pixels};
}

ImageWrapper::ImageWrapper(cv::Mat mat) : imageMat(std::move(mat)) {
  this->to_free = nullptr;
}

ImageWrapper::~ImageWrapper() {
  if (to_free != nullptr && *to_free != nullptr) {
    free(*to_free);
  }
  delete to_free;
}

const cv::Mat &ImageWrapper::GetImageMat() const {
  return imageMat;
}

