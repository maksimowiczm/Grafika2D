#include "image_adapter/imageSaver.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

void ImageSaver::save_jpeg(const char *file_path, const cv::Mat &image, int jpeg_quality) {
  std::vector<int> writeParams{};
  writeParams.push_back(cv::IMWRITE_JPEG_QUALITY);
  writeParams.push_back(jpeg_quality);

  cv::Mat out;
  cv::cvtColor(image, out, cv::COLOR_BGR2RGB);
  cv::imwrite(file_path, out, writeParams);
}
