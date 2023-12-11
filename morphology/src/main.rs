mod imgops;
mod ui;

use gtk::prelude::*;
use gtk::{glib, Application, ApplicationWindow};
use crate::ui::main_container::build_main_container;

const APP_ID: &str = "grafika.morfologia";

fn main() -> glib::ExitCode {
    let app = Application::builder().application_id(APP_ID).build();
    app.connect_activate(build_ui);
    app.run()
}

fn build_ui(app: &Application) {
    let window = ApplicationWindow::builder()
        .application(app)
        .title("Bezier")
        .child(&build_main_container())
        .build();

    window.present();
}
