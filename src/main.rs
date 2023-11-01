mod filters;
mod operations;
mod channels;
mod image;
mod containers;
mod context;

use std::cell::RefCell;
use std::rc::Rc;
use gtk::prelude::*;
use gtk::{glib, Application, ApplicationWindow};
use crate::containers::actions_container::build_actions_container;
use crate::context::Context;

const APP_ID: &str = "org.grafika.rust.obrazy";

fn main() -> glib::ExitCode {
    let app = Application::builder().application_id(APP_ID).build();
    app.connect_activate(build_ui);
    app.run()
}

fn build_ui(app: &Application) {
    let main_container = gtk::Box::new(gtk::Orientation::Horizontal, 10);
    main_container.set_size_request(800, 600);

    let window = ApplicationWindow::builder()
        .application(app)
        .title("Images reader")
        .child(&main_container)
        .build();

    let picture = gtk::Picture::new();
    let context = Rc::new(RefCell::new(Context { starting: None, mat: None }));

    main_container.append(&build_actions_container(&window, picture.clone(), Rc::clone(&context)));
    main_container.append(&picture);

    window.present();
}
