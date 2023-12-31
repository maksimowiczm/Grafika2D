#include "image_gui_gtk/window.hpp"

#include "gtkmm/messagedialog.h"

#include "image_adapter/imageSaver.hpp"
#include "image_adapter/imageLoader.hpp"

extern "C" {
#include "netpbm/PixMapReader.h"
#include "image_operations/operations.h"
}

ImageReaderWindow::ImageReaderWindow() {
  Gtk::Box container{Gtk::Orientation::HORIZONTAL};
  menu_ = Gtk::Box{Gtk::Orientation::VERTICAL, 5};
  container.append(menu_);

  load_button = Gtk::Button{"Load image"};
  load_button.signal_clicked().connect(sigc::mem_fun(*this, &ImageReaderWindow::handle_load_button_click));
  menu_.append(load_button);

  save_button = Gtk::Button{"Save image"};
  save_button.signal_clicked().connect(sigc::mem_fun(*this, &ImageReaderWindow::handle_save_button_click));
  menu_.append(save_button);

  Gtk::Box jpegScaler{Gtk::Orientation::HORIZONTAL};
  jpeg_scale_ = Gtk::Scale{Gtk::Adjustment::create(95, 0, 100)};
  jpeg_scale_.signal_value_changed().connect(sigc::mem_fun(*this, &ImageReaderWindow::on_scale_changed));
  jpeg_scale_.set_size_request(80, -1);
  jpegScaler.append(jpeg_scale_);
  jpeg_scale_label_ = Gtk::Label{};
  jpeg_scale_label_.set_text(std::to_string((int) jpeg_scale_.get_value()));
  jpegScaler.append(jpeg_scale_label_);

  menu_.append(jpegScaler);
  menu_.set_size_request(100, -1);

  picture = Gtk::Picture{};
  container.append(picture);

  filters_menu_ = Gtk::Box{Gtk::Orientation::VERTICAL};
  menu_.append(filters_menu_);
  setup_filters();

  operations_menu_ = Gtk::Box{Gtk::Orientation::VERTICAL};
  operation_entry = Gtk::Entry{};
  operations_menu_.append(operation_entry);
  menu_.append(operations_menu_);
  setup_operations();

  set_child(container);
}

void ImageReaderWindow::handle_load_button_click() {
  auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                           Gtk::FileChooser::Action::OPEN);
  dialog->set_transient_for(*this);
  dialog->set_modal(true);
  dialog->signal_response().connect(sigc::bind(
      sigc::mem_fun(*this, &ImageReaderWindow::on_file_dialog_response), dialog));

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

void ImageReaderWindow::on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog) {
  if (response_id != Gtk::ResponseType::OK) {
    delete dialog;
    return;
  }

  const auto file_path = dialog->get_file()->get_path();
  cv::Mat image;
  if (file_path.ends_with(".ppm")) {
    image = ImageLoader::load_image(file_path.c_str(), ImageLoader::PPM);
  } else {
    image = ImageLoader::load_image(file_path.c_str(), ImageLoader::OPEN_CV);
  }

  if (image.empty()) {
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

  delete imageContainer_;
  imageContainer_ = new ImageContainer(image);
  imageMat_ = image;
  image_draw();

  delete dialog;
}

void ImageReaderWindow::image_draw() {
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

void ImageReaderWindow::handle_save_button_click() {
  if (imageMat_.empty()) {
    return;
  }

  auto dialog = new Gtk::FileChooserDialog("Please choose a file",
                                           Gtk::FileChooser::Action::SAVE);
  dialog->set_transient_for(*this);
  dialog->set_modal(true);
  dialog->signal_response().connect(sigc::bind(
      sigc::mem_fun(*this, &ImageReaderWindow::on_save_file_dialog_response), dialog));

  dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
  dialog->add_button("_Save", Gtk::ResponseType::OK);

  dialog->show();
}

void ImageReaderWindow::on_save_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog) {
  if (response_id != Gtk::ResponseType::OK) {
    delete dialog;
    return;
  }

  const auto file_path = dialog->get_file()->get_path();
  ImageSaver::save_jpeg(file_path, imageMat_, (int) jpeg_scale_.get_value());
  delete dialog;
}

void ImageReaderWindow::on_scale_changed() {
  jpeg_scale_label_.set_text(std::to_string((int) jpeg_scale_.get_value()));
}

void ImageReaderWindow::handle_reset_filters_click() {
  if (imageContainer_ == nullptr) {
    return;
  }

  imageMat_ = imageContainer_->Get_image();
  image_draw();
}

void ImageReaderWindow::handle_filter_click(ImageContainer::Filter filter) {
  if (imageContainer_ == nullptr) {
    return;
  }

  imageMat_ = imageContainer_->Get_Filtered(filter);
  image_draw();
}

void ImageReaderWindow::setup_filter_button(const std::string &text, ImageContainer::Filter filter) {
  filters_.emplace_back(text);
  filters_[filters_.size() - 1]
      .signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &ImageReaderWindow::handle_filter_click), filter));
  filters_menu_.append(filters_[filters_.size() - 1]);
}

void ImageReaderWindow::setup_filters() {
  filters_.emplace_back("Reset");
  filters_[filters_.size() - 1]
      .signal_clicked().connect(sigc::mem_fun(*this, &ImageReaderWindow::handle_reset_filters_click));
  filters_menu_.append(filters_[filters_.size() - 1]);

  setup_filter_button("Mean filter", ImageContainer::Mean);
  setup_filter_button("Median filter", ImageContainer::Median);
  setup_filter_button("Sobel filter", ImageContainer::Sobel);
  setup_filter_button("HighPass filter", ImageContainer::HighPass);
  setup_filter_button("Gauss filter", ImageContainer::Gauss);
}

void ImageReaderWindow::setup_operations() {
  setup_operation_button("Brightness", ImageContainer::Brightness, operations_menu_);

  Gtk::Box add{Gtk::Orientation::HORIZONTAL};
  setup_operation_button("Add red", ImageContainer::Add, add, 0);
  setup_operation_button("Add green", ImageContainer::Add, add, 1);
  setup_operation_button("Add blue", ImageContainer::Add, add, 2);
  operations_menu_.append(add);

  Gtk::Box sub{Gtk::Orientation::HORIZONTAL};
  setup_operation_button("Subtract red", ImageContainer::Sub, sub, 0);
  setup_operation_button("Subtract green", ImageContainer::Sub, sub, 1);
  setup_operation_button("Subtract blue", ImageContainer::Sub, sub, 2);
  operations_menu_.append(sub);

  setup_operation_button("Multiply", ImageContainer::Multiply, operations_menu_);
  setup_operation_button("Divide", ImageContainer::Divide, operations_menu_);
  setup_operation_button("Gray mode", ImageContainer::GrayScale, operations_menu_);
}

void ImageReaderWindow::setup_operation_button(const std::string &text,
                                               ImageContainer::Operation operation,
                                               Gtk::Box &parent,
                                               size_t channel) {
  operations_.emplace_back(text);
  operations_[operations_.size() - 1]
      .signal_clicked().connect(
      sigc::bind(sigc::mem_fun(*this, &ImageReaderWindow::handle_operation_click), operation, channel));
  parent.append(operations_[operations_.size() - 1]);
}

void ImageReaderWindow::handle_operation_click(ImageContainer::Operation operation, size_t channel) {
  if (imageContainer_ == nullptr) {
    return;
  }

  try {
    auto text = std::stoi(operation_entry.get_buffer()->get_text());
    imageMat_ = imageContainer_->Get_Operated(imageMat_, operation, text, channel);
    image_draw();
  } catch (std::exception &e) {}
}
