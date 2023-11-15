mod bezier;
mod drawing_area;

use crate::bezier::BezierCurve;
use gtk::prelude::*;
use gtk::{glib, Application, ApplicationWindow};
use std::cell::RefCell;
use std::rc::Rc;

const APP_ID: &str = "grafik.bezier";

fn main() -> glib::ExitCode {
    let app = Application::builder().application_id(APP_ID).build();
    app.connect_activate(build_ui);
    app.run()
}

fn build_ui(app: &Application) {
    let container = gtk::Box::new(gtk::Orientation::Vertical, 0);
    let bezier = Rc::new(RefCell::new(BezierCurve::default()));

    let window = ApplicationWindow::builder()
        .application(app)
        .title("Bezier")
        .child(&container)
        .build();

    let drawing_area = drawing_area::build_drawing_area(Rc::clone(&bezier));
    container.append(&drawing_area);

    window.present();
}
