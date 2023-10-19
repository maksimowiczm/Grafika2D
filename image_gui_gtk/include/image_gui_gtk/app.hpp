#pragma once

#include "gtkmm/scale.h"
#include "gtkmm/window.h"
#include "gtkmm/filechooserdialog.h"
#include "gtkmm/button.h"
#include "gtkmm/picture.h"
#include "gtkmm/adjustment.h"
#include "gtkmm/label.h"

#include "opencv2/core/mat.hpp"

class ImageReaderApp : public Gtk::Window {
 public:
  ImageReaderApp();
  ~ImageReaderApp() override = default;

 private:
  cv::Mat imageMat_{};

  Gtk::Button load_button;
  void handle_load_button_click();

  Gtk::Button save_button;
  void handle_save_button_click();
  void on_save_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);
  Gtk::Scale jpeg_scale_;
  Gtk::Label jpeg_scale_label_;
  void on_scale_changed();

  void on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);

  Gtk::Picture picture;
  void image_draw();
};
