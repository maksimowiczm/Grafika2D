use gtk::prelude::*;
use gtk::{ApplicationWindow};

use std::cell::RefCell;
use std::rc::Rc;

use crate::context::{Context, picture_update_pixbuf};
use crate::{containers, image};

pub fn build_actions_container(window: &ApplicationWindow, picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Box {
    let actions_container = gtk::Box::new(gtk::Orientation::Vertical, 10);
    actions_container.append(&build_load_button(window.clone(), picture.clone(), Rc::clone(&context)));
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

fn build_load_button(window: ApplicationWindow, picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Button {
    let button = gtk::Button::with_label("Load image");
    button.connect_clicked(move |_| build_file_chooser(&window, picture.clone(), Rc::clone(&context)).show());
    button
}

fn build_file_chooser(window: &ApplicationWindow, picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::FileChooserDialog {
    let chooser = gtk::FileChooserDialog::new(
        Some("Load image"),
        Some(window),
        gtk::FileChooserAction::Open,
        &[("Open", gtk::ResponseType::Ok), ("Cancel", gtk::ResponseType::Cancel)],
    );

    chooser.connect_response(move |dialog, response| {
        if response != gtk::ResponseType::Ok {
            dialog.destroy();
        }

        let c_dialog = dialog.clone();
        let handle_error = || {
            let error = gtk::MessageDialog::new(Some(&c_dialog),
                                                gtk::DialogFlags::DESTROY_WITH_PARENT,
                                                gtk::MessageType::Error,
                                                gtk::ButtonsType::Ok,
                                                "Image format is not supported or file is corrupted!");
            error.connect_response(move |dialog, _| dialog.destroy());
            error.show();
        };

        // bro 💀
        if let Some(file) = dialog.file() {
            if let Some(path) = file.path() {
                if let Ok(out) = image::load_image(path.to_str().unwrap()) {
                    context.borrow_mut().mat = Some(out.clone());
                    context.borrow_mut().starting = Some(out.clone());
                    picture_update_pixbuf(&picture, &out);
                    dialog.destroy();
                } else {
                    handle_error();
                }
            } else {
                handle_error();
            }
        } else {
            handle_error();
        }
    });

    chooser
}
