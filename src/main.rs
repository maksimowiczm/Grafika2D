mod color_rgb;
mod color;
mod color_cmyk;

use std::cell::RefCell;
use std::rc::Rc;
use gtk::prelude::*;
use gtk::{glib, Application, ApplicationWindow, Box as GBox, Orientation, Scale, Adjustment, Entry, Label, Picture};
use gtk::gdk_pixbuf::{Colorspace, Pixbuf};
use gtk::glib::{Propagation};
use crate::color::Color;
use crate::color_cmyk::{CmykConverter, ColorCmyk};
use crate::color_rgb::{ColorRgb, RgbConverter};

const APP_ID: &str = "org.grafika";

#[derive(Debug)]
struct Colors {
    rgb: ColorRgb,
    cmyk: ColorCmyk,
    picture: Rc<Picture>,

    red: Option<GBox>,
    green: Option<GBox>,
    blue: Option<GBox>,

    cyan: Option<GBox>,
    magenta: Option<GBox>,
    yellow: Option<GBox>,
    key: Option<GBox>,
}

impl Colors {
    fn update(&mut self, rgb_input: bool) {
        if rgb_input {
            println!("rgb");
            let cmyk = self.rgb.convert_cmyk();

            let cyan_entry = self.cyan.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>();
            cyan_entry.unwrap().set_text(format!("{}", cmyk.0).as_str());
            let cyan_scale = self.cyan.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>();
            cyan_scale.unwrap().adjustment().set_value(cmyk.0 as f64);

            let magenta_entry = self.magenta.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>();
            magenta_entry.unwrap().set_text(format!("{}", cmyk.1).as_str());
            let magenta_scale = self.magenta.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>();
            magenta_scale.unwrap().adjustment().set_value(cmyk.1 as f64);

            let yellow_entry = self.yellow.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>();
            yellow_entry.unwrap().set_text(format!("{}", cmyk.2).as_str());
            let yellow_scale = self.yellow.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>();
            yellow_scale.unwrap().adjustment().set_value(cmyk.2 as f64);

            let key_entry = self.key.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>();
            key_entry.unwrap().set_text(format!("{}", cmyk.3).as_str());
            let key_scale = self.key.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>();
            key_scale.unwrap().adjustment().set_value(cmyk.3 as f64);

            let red = *self.rgb.red.borrow_mut();
            let green = *self.rgb.green.borrow_mut();
            let blue = *self.rgb.blue.borrow_mut();

            self.update_color(red, green, blue);
        } else {
            println!("cmyk");

            let rgb = self.cmyk.convert_rgb();

            let red_entry = self.red.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>();
            red_entry.unwrap().set_text(format!("{}", rgb.0).as_str());
            let red_scale = self.red.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>();
            red_scale.unwrap().adjustment().set_value(rgb.0 as f64);

            let green_entry = self.green.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>();
            green_entry.unwrap().set_text(format!("{}", rgb.1).as_str());
            let green_scale = self.green.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>();
            green_scale.unwrap().adjustment().set_value(rgb.1 as f64);

            let blue_entry = self.blue.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>();
            blue_entry.unwrap().set_text(format!("{}", rgb.2).as_str());
            let blue_scale = self.blue.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>();
            blue_scale.unwrap().adjustment().set_value(rgb.2 as f64);

            *self.rgb.red.borrow_mut() = rgb.0;
            *self.rgb.green.borrow_mut() = rgb.1;
            *self.rgb.blue.borrow_mut() = rgb.2;

            self.update_color(rgb.0, rgb.1, rgb.2);
        }
    }

    fn update_color(&mut self, red: u8, green: u8, blue: u8) {
        let pixbuf = Pixbuf::new(Colorspace::Rgb, false, 8, 1, 1);
        pixbuf.clone().unwrap().put_pixel(0, 0, red, green, blue, 0);
        self.picture.set_pixbuf(pixbuf.as_ref());
    }
}

fn main() -> glib::ExitCode {
    let app = Application::builder().application_id(APP_ID).build();
    app.connect_activate(build_ui);
    app.run()
}

fn color_scale_input<T: Color>(label_str: &str, colors: Rc<RefCell<Colors>>, r_value: Rc<RefCell<u8>>, rgb_input: bool) -> GBox {
    // Create gtk widgets
    let scale_container = GBox::new(Orientation::Horizontal, 0);
    let label = Label::builder().label(label_str).width_request(100).build();
    let adjustment = Adjustment::new(*r_value.borrow() as f64, 0., T::max_value() as f64 + 1., 1., 1., 1.);
    let scale = Scale::builder()
        .orientation(Orientation::Horizontal)
        .hexpand(true)
        .adjustment((&adjustment).into())
        .build();
    let buffer = Entry::builder().text(format!("{}", *r_value.borrow())).build();
    scale_container.append(&scale);
    scale_container.append(&label);
    scale_container.append(&buffer);

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

        Propagation::Proceed
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

fn build_ui(app: &Application) {
    let main_container = GBox::builder()
        .orientation(Orientation::Vertical)
        .width_request(1280)
        .height_request(720)
        .build();


    let picture = Picture::new();
    let pixbuf = Pixbuf::new(Colorspace::Rgb, false, 8, 1, 1);
    pixbuf.clone().unwrap().put_pixel(0, 0, 0, 0, 0, 0);
    picture.set_pixbuf(pixbuf.as_ref());

    let colors = Rc::new(RefCell::new(Colors {
        rgb: ColorRgb {
            red: Rc::new(RefCell::new(0)),
            green: Rc::new(RefCell::new(0)),
            blue: Rc::new(RefCell::new(0)),
        },
        cmyk: ColorCmyk {
            cyan: Rc::new(RefCell::new(0)),
            magenta: Rc::new(RefCell::new(0)),
            yellow: Rc::new(RefCell::new(0)),
            key: Rc::new(RefCell::new(100)),
        },
        picture: Rc::new(picture.clone()),
        red: None,
        green: None,
        blue: None,
        cyan: None,
        magenta: None,
        yellow: None,
        key: None,
    }));

    {
        let red = color_scale_input::<ColorRgb>("red", Rc::clone(&colors), Rc::clone(&colors.borrow().rgb.red), true);
        colors.borrow_mut().red = Some(red.clone());
        main_container.append(&red);

        let green = color_scale_input::<ColorRgb>("green", Rc::clone(&colors), Rc::clone(&colors.borrow().rgb.green), true);
        colors.borrow_mut().green = Some(green.clone());
        main_container.append(&green);

        let blue = color_scale_input::<ColorRgb>("blue", Rc::clone(&colors), Rc::clone(&colors.borrow().rgb.blue), true);
        colors.borrow_mut().blue = Some(blue.clone());
        main_container.append(&blue);
    }

    main_container.append(&picture);

    {
        let cyan = color_scale_input::<ColorCmyk>("cyan", Rc::clone(&colors), Rc::clone(&colors.borrow().cmyk.cyan), false);
        colors.borrow_mut().cyan = Some(cyan.clone());
        main_container.append(&cyan);

        let magenta = color_scale_input::<ColorCmyk>("magenta", Rc::clone(&colors), Rc::clone(&colors.borrow().cmyk.magenta), false);
        colors.borrow_mut().magenta = Some(magenta.clone());
        main_container.append(&magenta);

        let yellow = color_scale_input::<ColorCmyk>("yellow", Rc::clone(&colors), Rc::clone(&colors.borrow().cmyk.yellow), false);
        colors.borrow_mut().yellow = Some(yellow.clone());
        main_container.append(&yellow);

        let key = color_scale_input::<ColorCmyk>("key", Rc::clone(&colors), Rc::clone(&colors.borrow().cmyk.key), false);
        colors.borrow_mut().key = Some(key.clone());
        main_container.append(&key);
    }

    let window = ApplicationWindow::builder()
        .application(app)
        .title("My GTK App")
        .child(&main_container)
        .build();

    window.present();
}