#pragma once

#include "image_container.hpp"

#include "gtkmm/scale.h"
#include "gtkmm/window.h"
#include "gtkmm/filechooserdialog.h"
#include "gtkmm/button.h"
#include "gtkmm/picture.h"
#include "gtkmm/adjustment.h"
#include "gtkmm/label.h"
#include "gtkmm/entry.h"

#include "opencv2/core/mat.hpp"

class ImageReaderWindow : public Gtk::Window {
 public:
  ImageReaderWindow();
  ~ImageReaderWindow() override = default;

 private:
  cv::Mat imageMat_{};
  ImageContainer *imageContainer_ = nullptr;

  Gtk::Button load_button;
  void handle_load_button_click();

  Gtk::Button save_button;
  void handle_save_button_click();
  void on_save_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);
  Gtk::Scale jpeg_scale_;
  Gtk::Label jpeg_scale_label_;
  void on_scale_changed();

  void on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);

  Gtk::Box menu_;
  Gtk::Box filters_menu_;
  std::vector<Gtk::Button> filters_;
  void setup_filter_button(const std::string &text, ImageContainer::Filter filter);
  void handle_filter_click(ImageContainer::Filter filter);
  void handle_reset_filters_click();
  void setup_filters();

  Gtk::Entry operation_entry;
  std::vector<Gtk::Button> operations_;
  Gtk::Box operations_menu_;
  void handle_operation_click(ImageContainer::Operation operation, size_t channel);
  void setup_operation_button(const std::string &text,
                              ImageContainer::Operation operation,
                              Gtk::Box &parent,
                              size_t channel = 0);
  void setup_operations();

  Gtk::Picture picture;
  void image_draw();
};
