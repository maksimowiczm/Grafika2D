#include "imageViewer.hpp"

extern "C" {
#include "netpbm/PixMapReader.h"
}
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

static cv::Mat
load_PixMap(PixMapImage **ptr, const char *file_path) {
  *ptr = netpbm_PixMap_read_from_file(file_path);
  PixMapImage *image = *ptr;
  if (image == nullptr) {
    return {};
  }
  netpbm_convert_RGB_BGR(image);
  return {image->header.height, image->header.width, CV_8UC3, image->pixels};
}

inline static cv::Mat
load_OpenCV(const char *file_path) {
  return cv::imread(file_path, cv::IMREAD_COLOR);
}

bool ImageViewer::load_image(const char *file_path, ImageViewer::ImageType type) {
  cv::Mat image;
  void **toFree = new void *;
  *toFree = nullptr;

  if (type == PPM) {
    image = load_PixMap((PixMapImage **) toFree, file_path);
  } else if (type == OPEN_CV) {
    image = load_OpenCV(file_path);
  }

  if (image.empty()) {
    return false;
  }

  // scale to HD
  double scale = std::min(
      (double) 1280 / image.size().width,
      (double) 720 / image.size().height
  );

  cv::Mat out;
  cv::resize(image, out, cv::Size(), scale, scale, cv::INTER_AREA);
  cv::imshow(file_path, out);
  if (*toFree != nullptr) {
    free(*toFree);
  }
  delete toFree;

  return true;
}
