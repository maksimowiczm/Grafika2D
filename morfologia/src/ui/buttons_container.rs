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

    let operations: Vec<(&str, unsafe fn(&mut Mat, &Vec<Vec<u8>>))> = vec![
        ("dylatacja", Mat::dilatation),
        ("erozja", Mat::erosion),
        ("zamkniecie", Mat::closing),
        ("otwarcie", Mat::opening),
        ("pogrubienie", Mat::thickening),
        ("pocienianie", Mat::thinning),
    ];
    let mask_text = gtk::TextView::new();

    operations
        .iter()
        .map(|(label, action)| {
            build_operation_button(
                picture.clone(),
                Rc::clone(&context),
                label,
                *action,
                mask_text.clone(),
            )
        })
        .for_each(|button| {
            buttons_container.append(&button);
        });

    let mask = vec![vec![1, 1, 1]; 3];
    mask_text.buffer().set_text(
        &format!("{:?}", mask)
            .replace("[", "")
            .replace("], ", ",\n")
            .replace("]", "")
            .to_owned(),
    );
    buttons_container.append(&mask_text);

    buttons_container
}

fn build_operation_button(
    picture: gtk::Picture,
    context: Rc<RefCell<Context>>,
    label: &str,
    action: unsafe fn(&mut Mat, &Vec<Vec<u8>>),
    mask: gtk::TextView,
) -> gtk::Button {
    let button = gtk::Button::with_label(label);
    button.connect_clicked(move |_| unsafe {
        let buffer = mask.buffer();
        let (start, end) = buffer.bounds();
        let text = buffer.text(&start, &end, true);
        let mask = text
            .split("\n")
            .map(|line| {
                line.trim()
                    .split(",")
                    .flat_map(|v| v.trim().parse::<u8>())
                    .collect::<Vec<_>>()
            })
            .collect::<Vec<_>>();

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
