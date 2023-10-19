#include <gtkmm/messagedialog.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "image_gui_gtk/app.hpp"

#include "opencv2/core/mat.hpp"
#include "image_adapter/imageLoader.hpp"

extern "C" {
#include "netpbm/PixMapReader.h"
}

ImageReaderApp::ImageReaderApp() {
  Gtk::Box container{Gtk::Orientation::HORIZONTAL};
  Gtk::Box menu{Gtk::Orientation::VERTICAL};
  container.append(menu);

  load_button = Gtk::Button{"Load image"};
  load_button.signal_clicked().connect(sigc::mem_fun(*this, &ImageReaderApp::handle_load_button_click));
  menu.append(load_button);

  save_button = Gtk::Button{"Save image"};
  save_button.signal_clicked().connect(sigc::mem_fun(*this, &ImageReaderApp::handle_save_button_click));
  menu.append(save_button);

  picture = Gtk::Picture{};
  container.append(picture);

  set_child(container);
}

void ImageReaderApp::handle_load_button_click() {
  auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                           Gtk::FileChooser::Action::OPEN);
  dialog->set_transient_for(*this);
  dialog->set_modal(true);
  dialog->signal_response().connect(sigc::bind(
      sigc::mem_fun(*this, &ImageReaderApp::on_file_dialog_response), dialog));

  dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
  dialog->add_button("_Open", Gtk::ResponseType::OK);

  auto filter_ppm = Gtk::FileFilter::create();
  filter_ppm->set_name("PPM files");
  filter_ppm->add_pattern("*.ppm");
  dialog->add_filter(filter_ppm);

  auto filter_jpeg = Gtk::FileFilter::create();
  filter_jpeg->set_name("JPEG files");
  filter_jpeg->add_pattern("*.jpg");
  filter_jpeg->add_pattern("*.jpeg");
  dialog->add_filter(filter_jpeg);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog->add_filter(filter_any);

  dialog->show();
}

void ImageReaderApp::on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog) {
  if (response_id != Gtk::ResponseType::OK) {
    delete dialog;
    return;
  }

  const auto file_path = dialog->get_file()->get_path();
  if (file_path.ends_with(".ppm")) {
    imageMat_ = ImageLoader::load_image(file_path.c_str(), ImageLoader::PPM);
  } else {
    imageMat_ = ImageLoader::load_image(file_path.c_str(), ImageLoader::OPEN_CV);
  }

  if (imageMat_.empty()) {
    delete dialog;
    auto errorDialog = new Gtk::MessageDialog("Image format is not supported or file is corrupted!",
                                              false,
                                              Gtk::MessageType::ERROR,
                                              Gtk::ButtonsType::OK,
                                              true);
    errorDialog->set_title("File error");
    errorDialog->signal_response().connect(sigc::hide(sigc::mem_fun(*errorDialog, &Gtk::Widget::hide)));
    errorDialog->show();
    return;
  }

  image_draw();
  delete dialog;
}

void ImageReaderApp::image_draw() {
  const auto pixbuf = Gdk::Pixbuf::create_from_data(
      imageMat_.data,
      Gdk::Colorspace::RGB,
      false,
      8,
      imageMat_.size().width,
      imageMat_.size().height,
      imageMat_.size().width * sizeof(uint8_t) * 3
  );

  picture.set_pixbuf(pixbuf);
  picture.set_size_request(imageMat_.size().width,
                           imageMat_.size().height);
}

void ImageReaderApp::handle_save_button_click() {
  if (imageMat_.empty()) {
    return;
  }

  auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                           Gtk::FileChooser::Action::SAVE);
  dialog->set_transient_for(*this);
  dialog->set_modal(true);
  dialog->signal_response().connect(sigc::bind(
      sigc::mem_fun(*this, &ImageReaderApp::on_save_file_dialog_response), dialog));

  dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
  dialog->add_button("_Save", Gtk::ResponseType::OK);

  dialog->show();
}

void ImageReaderApp::on_save_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog) {
  if (response_id != Gtk::ResponseType::OK) {
    delete dialog;
    return;
  }

  const auto file_path = dialog->get_file()->get_path();

  std::vector<int> writeParams{};
  writeParams.push_back(cv::IMWRITE_JPEG_QUALITY);
  writeParams.push_back(jpegQuality);
  cv::Mat out;
  cv::cvtColor(imageMat_, out, cv::COLOR_BGR2RGB);
  cv::imwrite(file_path, out, writeParams);
  delete dialog;
}
