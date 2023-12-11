#include "gtkmm/application.h"

#include "image_gui_gtk/window.hpp"

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create("org.gtkmm.grafika");
  return app->make_window_and_run<ImageReaderWindow>(argc, argv);
}
