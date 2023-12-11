use crate::binarization::Binarization;
use crate::context::{picture_update_pixbuf, Context};
use gtk::prelude::*;
use std::cell::RefCell;
use std::rc::Rc;

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

    container.append(&build_binarize_button(picture, Rc::clone(&context), threshold_entry.clone()));
    container.append(&build_percent_black_selection_button(Rc::clone(&context), threshold_entry.clone()));
    container.append(&build_mean_iterative_selection_button(context, threshold_entry));

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

fn build_percent_black_selection_button(context: Rc<RefCell<Context>>, threshold_entry: gtk::Entry) -> gtk::Box {
    let container = gtk::Box::new(gtk::Orientation::Vertical, 0);
    let label = gtk::Label::with_mnemonic("Percent black selection");
    container.append(&label);

    let entry_container = gtk::Box::new(gtk::Orientation::Horizontal, 1);
    entry_container.append(&gtk::Label::with_mnemonic("Percent"));
    let entry = gtk::Entry::new();
    entry_container.append(&entry);
    container.append(&entry_container);

    let button = gtk::Button::with_label("Get threshold");
    container.append(&button);

    button.connect_clicked(move |_| {
        if let Some(mat) = &mut context.borrow_mut().mat {
            if let Ok(percent) = entry.buffer().text().parse::<f64>() {
                if percent > 100. || percent < 0. {
                    return;
                }
                let threshold = mat.percent_black_selection(percent / 100.).unwrap();
                threshold_entry.buffer().set_text(format!("{threshold}"));
            }
        }
    });

    container
}

fn build_mean_iterative_selection_button(context: Rc<RefCell<Context>>, threshold_entry: gtk::Entry) -> gtk::Box {
    let container = gtk::Box::new(gtk::Orientation::Vertical, 0);
    let label = gtk::Label::with_mnemonic("Mean iterative selection");
    container.append(&label);

    let button = gtk::Button::with_label("Get threshold");
    container.append(&button);

    button.connect_clicked(move |_| {
        if let Some(mat) = &mut context.borrow_mut().mat {
            let threshold = mat.mean_iterative_selection().unwrap();
            threshold_entry.buffer().set_text(format!("{threshold}"));
        }
    });

    container
}
