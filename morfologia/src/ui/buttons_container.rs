use crate::imgops::binary::ImageBinary;
use crate::imgops::r#unsafe::UnsafeHitOrMissMask;
use gtk::prelude::*;
use opencv::core::Mat;
use std::cell::RefCell;
use std::rc::Rc;

use crate::ui::context::{picture_update_pixbuf, Context};

pub fn build_buttons_container(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Box {
    let buttons_container = gtk::Box::new(gtk::Orientation::Vertical, 2);

    buttons_container.append(&build_load_button(picture.clone(), Rc::clone(&context)));
    buttons_container.append(&build_reset_button(picture.clone(), Rc::clone(&context)));

    let operations: Vec<(&str, unsafe fn(&mut Mat, &Vec<Vec<u8>>), Vec<Vec<u8>>)> = vec![
        ("dylatacja", Mat::dilatation, vec![vec![1, 1, 1]; 3]),
        ("erozja", Mat::erosion, vec![vec![1, 1, 1]; 3]),
        ("zamkniecie", Mat::closing, vec![vec![1, 1, 1]; 3]),
        ("otwarcie", Mat::opening, vec![vec![1, 1, 1]; 3]),
        (
            "pogrubienie",
            Mat::thickening,
            vec![vec![1, 1, 0], vec![1, 0, 0], vec![1, 0, 0]],
        ),
        ("pocienianie", Mat::thinning, vec![vec![1, 1, 1]; 3]),
    ];

    operations
        .iter()
        .map(|(label, action, mask)| {
            build_operation_button(
                picture.clone(),
                Rc::clone(&context),
                label,
                *action,
                mask.clone(),
            )
        })
        .for_each(|button| {
            buttons_container.append(&button);
        });

    buttons_container
}

fn build_operation_button(
    picture: gtk::Picture,
    context: Rc<RefCell<Context>>,
    label: &str,
    action: unsafe fn(&mut Mat, &Vec<Vec<u8>>),
    mask: Vec<Vec<u8>>,
) -> gtk::Button {
    let button = gtk::Button::with_label(label);
    button.connect_clicked(move |_| unsafe {
        let mut context = context.borrow_mut();
        if let Some(image) = &mut context.mat {
            action(image, &mask);
            picture_update_pixbuf(&picture, image);
        }
    });
    button
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

fn build_load_button(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Button {
    let button = gtk::Button::with_label("Load image");
    button
        .connect_clicked(move |_| build_file_chooser(picture.clone(), Rc::clone(&context)).show());
    button
}

fn build_file_chooser(
    picture: gtk::Picture,
    context: Rc<RefCell<Context>>,
) -> gtk::FileChooserDialog {
    let chooser = gtk::FileChooserDialog::builder()
        .action(gtk::FileChooserAction::Open)
        .build();
    chooser.add_buttons(&[
        ("Open", gtk::ResponseType::Ok),
        ("Cancel", gtk::ResponseType::Cancel),
    ]);

    chooser.connect_response(move |dialog, response| {
        if response != gtk::ResponseType::Ok {
            dialog.destroy();
        }

        let path = dialog.file().unwrap().path().unwrap();
        let path = path.to_str().unwrap();
        let out: Mat = ImageBinary::load(path).unwrap();
        context.borrow_mut().mat = Some(out.clone());
        context.borrow_mut().starting = Some(out.clone());
        picture_update_pixbuf(&picture, &out);
        dialog.destroy();
    });

    chooser
}
