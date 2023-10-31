#include "opencv2/core/mat.hpp"
#include "map"

class ImageContainer {
  cv::Mat starting_{};
 public:
  enum Filter {
    Mean,
    Median,
    Sobel,
    HighPass,
    Gauss
  };

  enum Operation {
    Add,
    Sub,
    Multiply,
    Divide,
    Brightness,
    GrayScale
  };

  explicit ImageContainer(cv::Mat &image);

  static cv::Mat Get_Operated(const cv::Mat &image, Operation operation, int value, size_t channel);

  cv::Mat Get_Filtered(enum Filter filter);
  cv::Mat Get_image();

 private:
  std::map<enum Filter, cv::Mat> filtered_{};
};
