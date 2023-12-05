mod color_rgb;
mod color;
mod color_cmyk;
mod scale_container;
mod colors;

use crate::colors::Colors;
use std::cell::RefCell;
use std::rc::Rc;
use gtk::prelude::*;
use gtk::{glib, Application, ApplicationWindow, Box as GBox, Orientation, Picture};
use gtk::gdk_pixbuf::{Colorspace, Pixbuf};
use crate::color_cmyk::ColorCmyk;
use crate::color_rgb::{ColorRgb};
use crate::scale_container::ScaleContainer;

const APP_ID: &str = "org.grafika";

fn main() -> glib::ExitCode {
    let app = Application::builder().application_id(APP_ID).build();
    app.connect_activate(build_ui);
    app.run()
}

fn build_ui(app: &Application) {
    let main_container = GBox::builder()
        .orientation(Orientation::Vertical)
        .width_request(1280)
        .height_request(720)
        .build();

    let pixbuf = Pixbuf::new(Colorspace::Rgb, false, 8, 1, 1);
    pixbuf.clone().unwrap().put_pixel(0, 0, 0, 0, 0, 0);

    let picture = Picture::new();
    picture.set_pixbuf(pixbuf.as_ref());

    let colors = Rc::new(RefCell::new(Colors::new(picture.clone())));

    let red = ScaleContainer::new::<ColorRgb>("red",
                                              Rc::clone(&colors),
                                              Rc::clone(&colors.borrow().rgb.red),
                                              true);
    colors.borrow_mut().red = Some(red.clone());

    let green = ScaleContainer::new::<ColorRgb>("green",
                                                Rc::clone(&colors),
                                                Rc::clone(&colors.borrow().rgb.green),
                                                true);
    colors.borrow_mut().green = Some(green.clone());

    let blue = ScaleContainer::new::<ColorRgb>("blue",
                                               Rc::clone(&colors),
                                               Rc::clone(&colors.borrow().rgb.blue),
                                               true);
    colors.borrow_mut().blue = Some(blue.clone());

    main_container.append(&red);
    main_container.append(&green);
    main_container.append(&blue);

    main_container.append(&picture);

    let cyan = ScaleContainer::new::<ColorCmyk>("cyan",
                                                Rc::clone(&colors),
                                                Rc::clone(&colors.borrow().cmyk.cyan),
                                                false);
    colors.borrow_mut().cyan = Some(cyan.clone());

    let magenta = ScaleContainer::new::<ColorCmyk>("magenta",
                                                   Rc::clone(&colors),
                                                   Rc::clone(&colors.borrow().cmyk.magenta),
                                                   false);
    colors.borrow_mut().magenta = Some(magenta.clone());

    let yellow = ScaleContainer::new::<ColorCmyk>("yellow",
                                                  Rc::clone(&colors),
                                                  Rc::clone(&colors.borrow().cmyk.yellow),
                                                  false);
    colors.borrow_mut().yellow = Some(yellow.clone());

    let key = ScaleContainer::new::<ColorCmyk>("key",
                                               Rc::clone(&colors),
                                               Rc::clone(&colors.borrow().cmyk.key),
                                               false);
    colors.borrow_mut().key = Some(key.clone());

    main_container.append(&cyan);
    main_container.append(&magenta);
    main_container.append(&yellow);
    main_container.append(&key);

    let window = ApplicationWindow::builder()
        .application(app)
        .title("My GTK App")
        .child(&main_container)
        .build();

    window.present();
}