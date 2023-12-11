use std::cell::RefCell;
use std::error::Error;
use std::rc::Rc;
use gtk::prelude::BoxExt;
use opencv::core::Mat;
use crate::containers::filter_container::build_filter_button;
use crate::context::Context;
use crate::histogram::Histogram;

pub fn build_histogram_container(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Box {
    let container = gtk::Box::new(gtk::Orientation::Vertical, 5);

    let filters: Vec<(&str, fn(&mut Mat) -> Result<(), Box<dyn Error>>)> = vec!(
        ("Normalize", Histogram::histogram_normalize),
        ("Equalize", Histogram::histogram_equalize),
    );

    filters.iter().for_each(|(label, func)| {
        let btn = build_filter_button(label, picture.clone(), Rc::clone(&context), *func);
        container.append(&btn)
    });

    container
}
