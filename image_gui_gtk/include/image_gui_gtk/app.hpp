#pragma once

#include "gtkmm/window.h"
#include "gtkmm/filechooserdialog.h"
#include "opencv2/core/mat.hpp"
#include "gtkmm/button.h"

class ImageReaderApp : public Gtk::Window {
 public:
  ImageReaderApp();
  ~ImageReaderApp() override;

 private:
  cv::Mat *image = nullptr;

  Gtk::Button load_button;
  void handle_load_button_click();

  void on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);
};
