use crate::ui::buttons_container::build_buttons_container;
use gtk::prelude::*;
use std::cell::RefCell;
use std::rc::Rc;

use crate::ui::context::Context;

pub fn build_main_container() -> gtk::Box {
    let container = gtk::Box::new(gtk::Orientation::Horizontal, 0);

    let context = Rc::new(RefCell::new(Context::default()));
    let picture = gtk::Picture::new();
    let buttons_container = build_buttons_container(picture.clone(), Rc::clone(&context));

    container.append(&buttons_container);
    container.append(&picture);

    container
}
