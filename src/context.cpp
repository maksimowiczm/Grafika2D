#include "context.hpp"

extern "C" {
#include "netpbm/PixMapReader.h"
}
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

inline static bool
load_PixMap(Context *context, const char *file_path) {
  PixMapImage *image = netpbm_PixMap_read_from_file(file_path);
  if (image == nullptr) {
    return false;
  }
  netpbm_convert_RGB_BGR(image);

  context->image.header.height = image->header.height;
  context->image.header.width = image->header.width;

  if (*context->image.to_free != nullptr) {
    free(*context->image.to_free);
  }
  *context->image.pixels = image->pixels;
  *context->image.to_free = image;

  return true;
}

inline static bool
load_JPEG(Context *context, const char *file_path) {
  cv::Mat mat = cv::imread(file_path, cv::IMREAD_COLOR);
  uint8_t *array = static_cast<uint8_t *>(malloc(sizeof(uint8_t) * mat.rows * mat.cols * mat.channels()));
  if (!mat.isContinuous()) {
    return false;
  }

  *array = *mat.data;
  if (*context->image.to_free != nullptr) {
    free(*context->image.to_free);
  }
  context->image.header.width = mat.cols;
  context->image.header.height = mat.rows;
  *context->image.pixels = array;
  *context->image.to_free = array;

  return true;
}

bool context_load_image(Context *context, const char *file_path, enum ImageType type) {
  switch (type) {
    case PPM:return load_PixMap(context, file_path);
    case JPG:return load_JPEG(context, file_path);
    default: return false;
  }
}