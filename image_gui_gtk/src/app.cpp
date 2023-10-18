#include "opencv2/core/mat.hpp"
#include "opencv2/imgcodecs.hpp"
#include "image_gui_gtk/app.hpp"
#include "opencv2/imgproc.hpp"

extern "C" {
#include "netpbm/PixMapReader.h"
}

ImageReaderApp::ImageReaderApp() {
  load_button = Gtk::Button{"Load image"};
  load_button.signal_clicked().connect(sigc::mem_fun(*this, &ImageReaderApp::handle_load_button_click));
  set_child(load_button);
}

ImageReaderApp::~ImageReaderApp() {
  delete image;
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

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog->add_filter(filter_any);

  //Show the dialog and wait for a user response:
  dialog->show();
}

void ImageReaderApp::on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog) {
  if (response_id != Gtk::ResponseType::OK) {
    delete dialog;
    return;
  }

  const auto file_path = dialog->get_file()->get_path();
  cv::Mat newImage;
  if (file_path.ends_with(".ppm")) {
    const auto read_image = netpbm_PixMap_read_from_file(file_path.c_str());
    if (read_image == nullptr) {
      delete dialog;
      return;
    }

    newImage = cv::Mat{read_image->header.height, read_image->header.width, CV_8UC3, read_image->pixels};
    free(read_image);
  } else {
    newImage = cv::Mat{cv::imread(file_path, cv::IMREAD_COLOR)};
  }

  // scale to HD
  double scale = std::min(
      (double) 1280 / newImage.size().width,
      (double) 720 / newImage.size().height
  );

  delete image;
  image = new cv::Mat;
  cv::resize(newImage, *image, cv::Size(), scale, scale, cv::INTER_AREA);

  delete dialog;
}
