#include "image_adapter/imageLoader.hpp"

extern "C" {
#include "netpbm/PixMapReader.h"
}
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

static cv::Mat
load_PixMap(PixMapImage **ptr, const char *file_path) {
  *ptr = netpbm_PixMap_read_from_file(file_path);
  PixMapImage *image = *ptr;
  if (image == nullptr) {
    return {};
  }

  return {image->header.height, image->header.width, CV_8UC3, image->pixels};
}

static cv::Mat
load_OpenCV(const char *file_path) {
  const auto mat = cv::imread(file_path, cv::IMREAD_COLOR);
  if (mat.empty()) {
    return {};
  }
  cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
  return mat;
}

cv::Mat ImageLoader::load_image(const char *file_path, ImageLoader::LoaderMethod type) {
  cv::Mat image;
  void **toFree = new void *;
  *toFree = nullptr;

  if (type == ImageLoader::PPM) {
    image = load_PixMap((PixMapImage **) toFree, file_path);
  } else if (type == ImageLoader::OPEN_CV) {
    image = load_OpenCV(file_path);
  }

  if (image.empty()) {
    return {};
  }

  // scale to HD
  double scale = std::min(
      (double) 1280 / image.size().width,
      (double) 720 / image.size().height
  );

  cv::Mat out;
  cv::resize(image, out, cv::Size(), scale, scale, cv::INTER_AREA);

  if (*toFree != nullptr) {
    free(*toFree);
  }
  delete toFree;

  return out;
}
