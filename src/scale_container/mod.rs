mod imp;

use std::cell::RefCell;
use std::rc::Rc;
use glib::Object;
use gtk::{glib};
use gtk::traits::RangeExt;
use crate::color::Color;
use crate::{Colors};
use gtk::prelude::*;


glib::wrapper! {
    pub struct ScaleContainer(ObjectSubclass<imp::ScaleContainer>)
        @extends gtk::Box, gtk::Widget,
        @implements gtk::Accessible, gtk::Actionable, gtk::Buildable, gtk::ConstraintTarget;
}

impl ScaleContainer {
    pub fn new<T: Color>(label_str: &str, colors: Rc<RefCell<Colors>>, r_value: Rc<RefCell<u8>>, rgb_input: bool) -> Self {
        let scale_container: ScaleContainer = Object::builder().build();
        let scale_box = scale_container.clone().dynamic_cast::<gtk::Box>().unwrap();
        scale_box.set_orientation(gtk::Orientation::Horizontal);

        // Create gtk widgets
        let label = gtk::Label::builder().label(label_str).width_request(100).build();
        let adjustment = gtk::Adjustment::new(*r_value.borrow() as f64, 0., T::max_value() as f64 + 1., 1., 1., 1.);
        let scale = gtk::Scale::builder()
            .orientation(gtk::Orientation::Horizontal)
            .hexpand(true)
            .adjustment((&adjustment).into())
            .build();
        let buffer = gtk::Entry::builder().text(format!("{}", *r_value.borrow())).build();
        scale_box.append(&scale);
        scale_box.append(&label);
        scale_box.append(&buffer);

        // connect signals

        // scale
        let c_buffer = buffer.clone();
        let c_colors = colors.clone();
        let c_r_value = r_value.clone();
        scale.connect_change_value(move |scale, _, _| {
            let value = scale.adjustment().value() as u8;
            c_buffer.set_text(value.to_string().as_str());
            *c_r_value.borrow_mut() = value;
            c_colors.borrow_mut().update(rgb_input);

            glib::Propagation::Proceed
        });

        // input
        let c_scale = scale.clone();
        buffer.connect_activate(move |entry| {
            let input = entry.buffer().text();
            let value_result = input.parse::<u32>();
            if value_result.is_err() {
                entry.buffer().set_text("0");
                *r_value.borrow_mut() = 0;
                colors.borrow_mut().update(rgb_input);
                return;
            }

            let value = value_result.unwrap();
            if value > T::max_value() as u32 {
                entry.buffer().set_text(format!("{}", T::max_value()));
            }

            *r_value.borrow_mut() = value as u8;
            c_scale.adjustment().set_value(value as f64);
            colors.borrow_mut().update(rgb_input);
        });

        scale_container
    }
}
