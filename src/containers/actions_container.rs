use gtk::prelude::*;

use std::cell::RefCell;
use std::rc::Rc;

use crate::context::{Context, picture_update_pixbuf};
use crate::{containers};

pub fn build_actions_container(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Box {
    let actions_container = gtk::Box::new(gtk::Orientation::Vertical, 10);
    actions_container.append(&build_reset_button(picture.clone(), Rc::clone(&context)));
    actions_container.append(&containers::filter_container::build_filters_container(picture.clone(), Rc::clone(&context)));
    actions_container.append(&containers::operations_container::build_operations_container(picture.clone(), Rc::clone(&context)));
    actions_container
}

fn build_reset_button(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Button {
    let button = gtk::Button::with_label("Reset image");
    button.connect_clicked(move |_| {
        let mut c_context = context.borrow_mut();
        if let Some(mat) = &c_context.starting {
            picture_update_pixbuf(&picture, &mat);
            c_context.mat = Some(mat.clone());
        }
    });
    button
}
