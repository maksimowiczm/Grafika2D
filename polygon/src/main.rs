mod drawing;
mod polygon;

use drawing::build_drawing_area_container;
use gtk::prelude::*;
use gtk::{glib, Application, ApplicationWindow};

const APP_ID: &str = "org.grafika.polygon";

fn main() -> glib::ExitCode {
    let app = Application::builder().application_id(APP_ID).build();
    app.connect_activate(build_ui);
    app.run()
}

fn build_ui(app: &Application) {
    let container = gtk::Box::new(gtk::Orientation::Vertical, 0);

    let window = ApplicationWindow::builder()
        .application(app)
        .title("Polygon")
        .child(&container)
        .build();

    container.append(&build_drawing_area_container(&window));

    window.present();
}
