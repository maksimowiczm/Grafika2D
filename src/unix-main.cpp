#include "gtkmm/application.h"

#include "image_gui_gtk/app.hpp"

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create("org.gtkmm.grafika");
  return app->make_window_and_run<ImageReaderApp>(argc, argv);
}
