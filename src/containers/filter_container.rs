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

    filters_container.append(&build_custom_filter_button(picture, Rc::clone(&context)));

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

fn build_custom_filter_button(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Button {
    let button = gtk::Button::with_label("Custom");

    button.connect_clicked(move |_| {
        if context.borrow().mat.is_none() {
            return;
        }
        let window = build_custom_filter_window(picture.clone(), Rc::clone(&context));
        window.show();
    });

    button
}

fn build_custom_filter_window(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Window {
    let container = gtk::Box::new(gtk::Orientation::Vertical, 10);
    let entry = gtk::TextView::builder()
        .margin_top(10)
        .margin_start(10)
        .margin_end(10)
        .margin_bottom(10)
        .hexpand(true)
        .vexpand(true)
        .build();
    container.append(&entry);

    let apply_button = gtk::Button::builder()
        .label("Apply")
        .margin_start(10)
        .margin_end(10)
        .margin_bottom(10)
        .build();
    container.append(&apply_button);

    apply_button.connect_clicked(move |_| {
        let buffer = entry.buffer();
        let (start, end) = buffer.bounds();
        let text = String::from(buffer.text(&start, &end, false));
        if let Some(mask) = parse_mask(text.as_str()) {
            if let Some(mat) = &mut context.borrow_mut().mat {
                mat.mask_filter(1., mask).unwrap();
                crate::context::picture_update_pixbuf(&picture, &mat);
            }
        }
    });

    let window = gtk::Window::builder()
        .title("Custom filter")
        .child(&container)
        .width_request(300)
        .height_request(300)
        .build();

    window
}

fn parse_mask(input: &str) -> Option<Vec<Vec<f64>>> {
    let buff: Vec<f64> = input
        .split(",")
        .map(|val| val
            .chars()
            .filter(|&c| !c.is_whitespace())
            .collect::<String>()
            .parse::<f64>()
        )
        .flatten()
        .collect();

    let size = buff.len() as f64;
    let sqrt = size.sqrt();
    if sqrt.fract() == 0. && sqrt % 2. == 1. {
        let mut out = Vec::new();
        let size = sqrt as usize;
        for i in 0..size {
            let mut row = Vec::new();

            for j in 0..size {
                row.push(buff[i * size + j]);
            }

            out.push(row);
        }

        Some(out)
    } else {
        None
    }
}