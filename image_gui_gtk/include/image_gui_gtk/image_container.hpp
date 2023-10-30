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

  explicit ImageContainer(cv::Mat &image);

  cv::Mat Get_Filtered(enum Filter filter);
  cv::Mat Get_image();

 private:
  std::map<enum Filter, cv::Mat> filtered_{};
};
