#pragma once

#include "opencv2/core/mat.hpp"

#include "gtkmm/window.h"
#include "gtkmm/filechooserdialog.h"
#include "gtkmm/button.h"
#include "gtkmm/picture.h"
#include "ImageWrapper.hpp"

class ImageReaderApp : public Gtk::Window {
 public:
  ImageReaderApp();
  ~ImageReaderApp() override;

 private:
  ImageWrapper *imageWrapper_ = nullptr;

  Gtk::Button load_button;
  void handle_load_button_click();

  void on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);

  Gtk::Picture picture;
  void image_draw(ImageWrapper *newImage);
};
