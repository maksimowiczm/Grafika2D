use gtk::prelude::*;

use std::cell::RefCell;
use std::rc::Rc;

use crate::context::{Context};
use crate::{containers};

pub fn build_actions_container(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Box {
    let actions_container = gtk::Box::new(gtk::Orientation::Vertical, 10);
    actions_container.append(&containers::filter_container::build_filters_container(picture.clone(), Rc::clone(&context)));
    actions_container.append(&containers::operations_container::build_operations_container(picture.clone(), Rc::clone(&context)));
    actions_container
}
