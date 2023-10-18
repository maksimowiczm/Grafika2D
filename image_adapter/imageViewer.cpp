#include "image_adapter/imageViewer.hpp"

#include "opencv2/highgui.hpp"

bool ImageViewer::load_image(const char *file_path, enum ImageLoader::LoaderMethod type) {
  cv::Mat image = ImageLoader::load_image(file_path, type);
  cv::imshow(file_path, image);
  return true;
}
