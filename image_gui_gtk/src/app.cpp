#include "opencv2/core/mat.hpp"
#include "opencv2/imgcodecs.hpp"
#include "image_gui_gtk/app.hpp"
#include "opencv2/imgproc.hpp"

extern "C" {
#include "netpbm/PixMapReader.h"
}

ImageReaderApp::ImageReaderApp() {
  Gtk::Box container{};

  load_button = Gtk::Button{"Load imageMat"};
  load_button.signal_clicked().connect(sigc::mem_fun(*this, &ImageReaderApp::handle_load_button_click));
  container.append(load_button);

  image = Gtk::Image{};
  container.append(image);
  image.set_vexpand();
  image.set_vexpand_set();

  set_size_request(1280, 720);
  set_child(container);

  to_free = new void *;
  *to_free = nullptr;
}

ImageReaderApp::~ImageReaderApp() {
  delete imageMat;
  if (*to_free != nullptr) {
    free(*to_free);
  }
  delete to_free;
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

    if (*to_free != nullptr) {
      free(*to_free);
    }

    *to_free = read_image;
    newImage = cv::Mat{read_image->header.height, read_image->header.width, CV_8UC3, read_image->pixels};
  } else {
    newImage = cv::Mat{cv::imread(file_path, cv::IMREAD_COLOR)};
  }

  image_draw(newImage);

  delete dialog;
}

void ImageReaderApp::image_draw(const cv::Mat &newImage) {
  // scale to HD
  double scale = std::min(
      (double) 1280 / newImage.size().width,
      (double) 720 / newImage.size().height
  );

  delete imageMat;
  imageMat = new cv::Mat;
  cv::resize(newImage, *imageMat, cv::Size(), scale, scale, cv::INTER_AREA);

  const auto pixbuf =
      Gdk::Pixbuf::create_from_data(
          imageMat->data,
          Gdk::Colorspace::RGB,
          false,
          8,
          imageMat->size().width,
          imageMat->size().height,
          imageMat->size().width * sizeof(uint8_t) * 3
      );

  this->image.set(pixbuf);
  this->image.set_size_request(1000, 720);
}
