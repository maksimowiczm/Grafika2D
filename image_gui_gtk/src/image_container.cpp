#include "image_gui_gtk/image_container.hpp"
extern "C" {
#include "image_filters/filters.h"
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
