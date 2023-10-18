#pragma once

#include "opencv2/core/mat.hpp"

#include "gtkmm/window.h"
#include "gtkmm/filechooserdialog.h"
#include "gtkmm/button.h"
#include "gtkmm/image.h"

class ImageReaderApp : public Gtk::Window {
 public:
  ImageReaderApp();
  ~ImageReaderApp() override;

 private:
  void **to_free;

  cv::Mat *imageMat = nullptr;

  Gtk::Button load_button;
  void handle_load_button_click();

  void on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);

  Gtk::Image image;
  void image_draw(const cv::Mat& newImage);
};
