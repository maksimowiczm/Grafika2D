#include "image_gui_gtk/app.hpp"

#include "opencv2/core/mat.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

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

  picture = Gtk::Picture{};
  container.append(picture);

//  set_size_request(1280, 720);
  set_child(container);
}

ImageReaderApp::~ImageReaderApp() {
  delete this->imageWrapper_;
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
  ImageWrapper *newImage;
  if (file_path.ends_with(".ppm")) {
    const auto read_image = netpbm_PixMap_read_from_file(file_path.c_str());
    if (read_image == nullptr) {
      delete dialog;
      return;
    }

    newImage = new ImageWrapper{read_image->header.width, read_image->header.height, read_image->pixels, read_image};

  } else {
    newImage = new ImageWrapper{cv::Mat{cv::imread(file_path, cv::IMREAD_COLOR)}};
  }

  image_draw(newImage);

  delete dialog;
}

void ImageReaderApp::image_draw(ImageWrapper *newImage) {
  // scale to HD
  double scale = std::min(
      (double) 1280 / newImage->GetImageMat().size().width,
      (double) 720 / newImage->GetImageMat().size().height
  );

  delete imageWrapper_;
  cv::Mat scaled;
  cv::resize(newImage->GetImageMat(), scaled, cv::Size(), scale, scale, cv::INTER_AREA);
  delete newImage;
  imageWrapper_ = new ImageWrapper{scaled};

  const auto pixbuf =
      Gdk::Pixbuf::create_from_data(
          imageWrapper_->GetImageMat().data,
          Gdk::Colorspace::RGB,
          false,
          8,
          imageWrapper_->GetImageMat().size().width,
          imageWrapper_->GetImageMat().size().height,
          imageWrapper_->GetImageMat().size().width * sizeof(uint8_t) * 3
      );

  picture.set_pixbuf(pixbuf);
  picture.set_size_request(imageWrapper_->GetImageMat().size().width,
                           imageWrapper_->GetImageMat().size().height);
}
