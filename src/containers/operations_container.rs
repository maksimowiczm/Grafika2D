use opencv::{prelude::*};

use gtk::prelude::*;

use std::cell::RefCell;
use std::rc::Rc;

use crate::context::{Context};
use crate::operations::Operations;

pub fn build_operations_container(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Box {
    let container = gtk::Box::new(gtk::Orientation::Vertical, 5);

    let label = gtk::Label::builder()
        .label("Operations")
        .build();
    container.append(&label);

    container.append(&build_channel_operation_container("Add",
                                                        picture.clone(),
                                                        Rc::clone(&context),
                                                        Operations::add_to_channel));

    container.append(&build_operation_container("Brightness",
                                                picture.clone(),
                                                Rc::clone(&context),
                                                Operations::add_brightness));

    container
}

fn build_operation_container(label: &str,
                             picture: gtk::Picture,
                             context: Rc<RefCell<Context>>,
                             method: fn(&mut Mat, i16) -> Result<(), opencv::Error>) -> gtk::Box {
    let container = gtk::Box::builder()
        .orientation(gtk::Orientation::Vertical)
        .spacing(1)
        .margin_start(5)
        .margin_end(5)
        .build();
    let entry = gtk::Entry::new();
    let btn = gtk::Button::with_label("Set");

    let c_entry = entry.clone();
    btn.connect_clicked(move |_| {
        if let Some(mat) = &mut context.borrow_mut().mat {
            if let Ok(value) = c_entry.buffer().text().parse::<i16>() {
                method(mat, value).unwrap();
                crate::context::picture_update_pixbuf(&picture, &mat);
            }
        }
    });

    container.append(&gtk::Label::builder().label(label).build());
    container.append(&entry.clone());
    container.append(&btn.clone());

    container
}

fn build_channel_operation_container(label: &str,
                                     picture: gtk::Picture,
                                     context: Rc<RefCell<Context>>,
                                     method: fn(&mut Mat, i16, usize) -> Result<(), opencv::Error>) -> gtk::Box {
    let container = gtk::Box::builder()
        .orientation(gtk::Orientation::Vertical)
        .spacing(1)
        .margin_start(5)
        .margin_end(5)
        .build();

    let entries = gtk::Box::new(gtk::Orientation::Horizontal, 1);
    entries.append(&gtk::Label::builder()
        .label(label)
        .margin_end(2)
        .build());
    let entry = gtk::Entry::new();
    entries.append(&entry);
    container.append(&entries);

    let buttons = gtk::Box::new(gtk::Orientation::Horizontal, 1);
    container.append(&buttons);

    let btn_red = build_channel_operation_button("red", entry.clone(), picture.clone(), Rc::clone(&context), method, 0);
    let btn_green = build_channel_operation_button("green", entry.clone(), picture.clone(), Rc::clone(&context), method, 1);
    let btn_blue = build_channel_operation_button("blue", entry.clone(), picture.clone(), Rc::clone(&context), method, 2);

    buttons.append(&btn_red.clone());
    buttons.append(&btn_green.clone());
    buttons.append(&btn_blue.clone());
    container
}

fn build_channel_operation_button(label: &str,
                                  entry: gtk::Entry,
                                  picture: gtk::Picture,
                                  context: Rc<RefCell<Context>>,
                                  method: fn(&mut Mat, i16, usize) -> Result<(), opencv::Error>,
                                  channel: usize) -> gtk::Button {
    let btn = gtk::Button::with_label(label);
    btn.connect_clicked(move |_| {
        if let Some(mat) = &mut context.borrow_mut().mat {
            if let Ok(value) = entry.buffer().text().parse::<i16>() {
                method(mat, value, channel).unwrap();
                crate::context::picture_update_pixbuf(&picture, &mat);
            }
        }
    });

    btn
}
