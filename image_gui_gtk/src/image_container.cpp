#include "image_gui_gtk/image_container.hpp"
extern "C" {
#include "image_filters/filters.h"
#include "image_operations/operations.h"
}

ImageContainer::ImageContainer(cv::Mat &image) : starting_(image) {}

auto Filter(cv::Mat &image, ImageContainer::Filter filterType) -> void {
  switch (filterType) {
    case ImageContainer::Mean:
      return image_filters_mean_filter(image.data,
                                       image.size().width,
                                       image.size().height,
                                       3,
                                       5);
    case ImageContainer::Median:
      return image_filters_median_filter(image.data,
                                         image.size().width,
                                         image.size().height,
                                         3,
                                         5);
    case ImageContainer::Sobel:
      return image_filters_sobel_filter(image.data,
                                        image.size().width,
                                        image.size().height,
                                        3,
                                        5);
    case ImageContainer::HighPass:
      return image_filters_highpass_filter(image.data,
                                           image.size().width,
                                           image.size().height,
                                           3,
                                           5);
    case ImageContainer::Gauss:
      return image_filters_gauss_filter(image.data,
                                        image.size().width,
                                        image.size().height,
                                        3,
                                        5);
  }
}

cv::Mat ImageContainer::Get_Filtered(enum Filter filter) {
  if (filtered_.contains(filter)) {
    return filtered_[filter];
  }

  auto filtered = this->starting_.clone();
  ::Filter(filtered, filter);

  filtered_[filter] = filtered;

  return filtered;
}

cv::Mat ImageContainer::Get_image() {
  return starting_;
}

auto Operate(cv::Mat &image, ImageContainer::Operation type, int16_t value, size_t channel) -> void {
  switch (type) {
    case ImageContainer::Add:
      return image_operations_add(image.data,
                                  image.total() * image.channels(),
                                  image.channels(),
                                  channel,
                                  value);
    case ImageContainer::Sub:
      return image_operations_add(image.data,
                                  image.total() * image.channels(),
                                  image.channels(),
                                  channel,
                                  -value);
    case ImageContainer::Multiply:break;
    case ImageContainer::Divide:break;
    case ImageContainer::Brightness:
      return image_operations_brightness(image.data,
                                         image.total() * image.channels(),
                                         value);
    case ImageContainer::GrayScale:break;
  }
}

cv::Mat ImageContainer::Get_Operated(const cv::Mat &image, Operation operation, int value, size_t channel) {
  auto filtered = image.clone();
  ::Operate(filtered, operation, value, channel);
  return filtered;
}
