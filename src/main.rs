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

    let key_controller = gtk::EventControllerKey::new();
    key_controller.connect_key_pressed(move |_, _, key, _| match key {
        27 | 9 => {
            bezier.borrow_mut().selected = None;
            bezier.borrow_mut().points.clear();
            drawing_area.queue_draw();
            return glib::Propagation::Stop;
        }
        _ => glib::Propagation::Stop,
    });
    window.add_controller(key_controller);

    window.present();
}
