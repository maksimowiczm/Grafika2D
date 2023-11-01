use opencv::{prelude::*};

use gtk::prelude::*;

use std::cell::RefCell;
use std::rc::Rc;
use std::error::Error;

use crate::context::Context;
use crate::filters::Filters;

pub(crate) fn build_filters_container(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Box {
    let filters_container = gtk::Box::new(gtk::Orientation::Vertical, 5);

    let filters: Vec<(&str, fn(&mut Mat) -> Result<(), Box<dyn Error>>)> = vec!(
        ("Mean", |mat| mat.mean_filter(3)),
        ("High pass", |mat| mat.high_pass_filter(3)),
        ("Sobel", Filters::sobel_filter),
        ("Gauss", Filters::gauss_filter),
    );

    filters.iter().for_each(|(label, func)| {
        let btn = build_filter_button(label, picture.clone(), Rc::clone(&context), *func);
        filters_container.append(&btn)
    });

    filters_container
}

fn build_filter_button(label: &str, picture: gtk::Picture, context: Rc<RefCell<Context>>, method: fn(&mut Mat) -> Result<(), Box<dyn Error>>) -> gtk::Button {
    let button = gtk::Button::with_label(label);
    button.connect_clicked(move |_| {
        if let Some(mat) = &mut context.borrow_mut().mat {
            method(mat).unwrap();
            crate::context::picture_update_pixbuf(&picture, &mat);
        }
    });
    button
}
