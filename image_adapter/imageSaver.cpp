#include "image_adapter/imageSaver.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

void ImageSaver::save_jpeg(const std::string &file_path, const cv::Mat &image, int jpeg_quality) {
  std::vector<int> writeParams{};
  writeParams.push_back(cv::IMWRITE_JPEG_QUALITY);
  writeParams.push_back(jpeg_quality);

  std::string path{file_path};
  if (!file_path.ends_with(".jpeg")) {
    path += ".jpeg";
  }
  cv::Mat out;
  cv::cvtColor(image, out, cv::COLOR_BGR2RGB);
  cv::imwrite(path, out, writeParams);
}
