#pragma once

#include <gtkmm/window.h>
#include "gtkmm/button.h"

class ImageReaderApp : public Gtk::Window {
 public:
  ImageReaderApp();
  ~ImageReaderApp() override;
};
