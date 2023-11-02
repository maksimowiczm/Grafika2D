use gtk::prelude::*;

use std::cell::RefCell;
use std::rc::Rc;

use crate::context::{Context};

pub fn build_operations_container(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Box {
    let container = gtk::Box::new(gtk::Orientation::Vertical, 5);

    let label = gtk::Label::builder()
        .label("Operations")
        .build();
    container.append(&label);

    container
}
