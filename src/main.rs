mod color_rgb;
mod color;
mod color_cmyk;
mod scale_container;

use std::cell::RefCell;
use std::rc::Rc;
use gtk::prelude::*;
use gtk::{glib, Application, ApplicationWindow, Box as GBox, Orientation, Picture, Entry, Scale};
use gtk::gdk_pixbuf::{Colorspace, Pixbuf};
use crate::color_cmyk::{CmykConverter, ColorCmyk};
use crate::color_rgb::{ColorRgb, RgbConverter};
use crate::scale_container::ScaleContainer;

const APP_ID: &str = "org.grafika";

#[derive(Debug)]
pub struct Colors {
    rgb: ColorRgb,
    cmyk: ColorCmyk,
    picture: Rc<Picture>,

    red: Option<ScaleContainer>,
    green: Option<ScaleContainer>,
    blue: Option<ScaleContainer>,

    cyan: Option<ScaleContainer>,
    magenta: Option<ScaleContainer>,
    yellow: Option<ScaleContainer>,
    key: Option<ScaleContainer>,
}

fn update_entry_scale(entry: Entry, scale: Scale, value: u8) {
    entry.set_text(format!("{}", value).as_str());
    scale.adjustment().set_value(value as f64);
}

impl Colors {
    fn rgb_changed(&mut self) -> (u8, u8, u8) {
        let cmyk = self.rgb.convert_cmyk();

        update_entry_scale(self.cyan.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>().unwrap(),
                           self.cyan.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>().unwrap(),
                           cmyk.0);

        update_entry_scale(self.magenta.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>().unwrap(),
                           self.magenta.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>().unwrap(),
                           cmyk.1);

        update_entry_scale(self.yellow.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>().unwrap(),
                           self.yellow.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>().unwrap(),
                           cmyk.2);

        update_entry_scale(self.key.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>().unwrap(),
                           self.key.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>().unwrap(),
                           cmyk.3);

        *self.cmyk.cyan.borrow_mut() = cmyk.0;
        *self.cmyk.magenta.borrow_mut() = cmyk.1;
        *self.cmyk.yellow.borrow_mut() = cmyk.2;
        *self.cmyk.key.borrow_mut() = cmyk.3;

        (*self.rgb.red.borrow_mut(), *self.rgb.green.borrow_mut(), *self.rgb.blue.borrow_mut())
    }
    fn cmyk_changed(&mut self) -> (u8, u8, u8) {
        let rgb = self.cmyk.convert_rgb();

        update_entry_scale(self.red.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>().unwrap(),
                           self.red.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>().unwrap(),
                           rgb.0);

        update_entry_scale(self.green.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>().unwrap(),
                           self.green.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>().unwrap(),
                           rgb.1);

        update_entry_scale(self.blue.clone().unwrap().last_child().unwrap().dynamic_cast::<Entry>().unwrap(),
                           self.blue.clone().unwrap().first_child().unwrap().dynamic_cast::<Scale>().unwrap(),
                           rgb.2);

        *self.rgb.red.borrow_mut() = rgb.0;
        *self.rgb.green.borrow_mut() = rgb.1;
        *self.rgb.blue.borrow_mut() = rgb.2;

        rgb
    }
    fn update(&mut self, rgb_input: bool) {
        let (red, green, blue) = if rgb_input { self.rgb_changed() } else { self.cmyk_changed() };
        self.update_color(red, green, blue);
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

    let red =
        ScaleContainer::new::<ColorRgb>("red",
                                        Rc::clone(&colors),
                                        Rc::clone(&colors.borrow().rgb.red),
                                        true);
    colors.borrow_mut().red = Some(red.clone());

    let green =
        ScaleContainer::new::<ColorRgb>("green",
                                        Rc::clone(&colors),
                                        Rc::clone(&colors.borrow().rgb.green),
                                        true);
    colors.borrow_mut().green = Some(green.clone());

    let blue =
        ScaleContainer::new::<ColorRgb>("blue",
                                        Rc::clone(&colors),
                                        Rc::clone(&colors.borrow().rgb.blue),
                                        true);
    colors.borrow_mut().blue = Some(blue.clone());

    main_container.append(&red);
    main_container.append(&green);
    main_container.append(&blue);

    main_container.append(&picture);

    let cyan =
        ScaleContainer::new::<ColorCmyk>("cyan",
                                         Rc::clone(&colors),
                                         Rc::clone(&colors.borrow().cmyk.cyan),
                                         false);
    colors.borrow_mut().cyan = Some(cyan.clone());

    let magenta =
        ScaleContainer::new::<ColorCmyk>("magenta",
                                         Rc::clone(&colors),
                                         Rc::clone(&colors.borrow().cmyk.magenta),
                                         false);
    colors.borrow_mut().magenta = Some(magenta.clone());

    let yellow =
        ScaleContainer::new::<ColorCmyk>("yellow",
                                         Rc::clone(&colors),
                                         Rc::clone(&colors.borrow().cmyk.yellow),
                                         false);
    colors.borrow_mut().yellow = Some(yellow.clone());

    let key =
        ScaleContainer::new::<ColorCmyk>("key",
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