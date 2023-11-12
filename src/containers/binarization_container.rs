use std::cell::RefCell;
use std::rc::Rc;
use gtk::prelude::*;
use crate::binarization::Binarization;
use crate::context::{Context, picture_update_pixbuf};

pub fn build_binarization_container(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Box {
    let container = gtk::Box::new(gtk::Orientation::Vertical, 5);

    let label = gtk::Label::builder().label("Binarization").build();
    container.append(&label);

    let binarize_container = gtk::Box::new(gtk::Orientation::Horizontal, 10);

    let label = gtk::Label::builder().label("Threshold").build();
    binarize_container.append(&label);

    let threshold_entry = gtk::Entry::new();
    binarize_container.append(&threshold_entry);

    container.append(&binarize_container);

    container.append(&build_binarize_button(picture, context, threshold_entry));

    container
}

fn build_binarize_button(picture: gtk::Picture, context: Rc<RefCell<Context>>, threshold_entry: gtk::Entry) -> gtk::Button {
    let button = gtk::Button::with_label("Binarize");

    button
        .connect_clicked(move |_| {
            if let Some(mat) = &mut context.borrow_mut().mat {
                let text = threshold_entry.buffer().text();
                if let Ok(threshold) = text.parse::<u8>() {
                    mat.threshold_binarization(threshold).unwrap();
                    picture_update_pixbuf(&picture, mat);
                }
            }
        });

    button
}
