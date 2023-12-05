use std::cell::RefCell;
use std::rc::Rc;
use gtk::Picture;
use crate::color_cmyk::ColorCmyk;
use crate::color_rgb::ColorRgb;
use crate::scale_container::ScaleContainer;
use gtk::gdk_pixbuf::{Colorspace, Pixbuf};
use crate::color_cmyk::CmykConverter;
use crate::color_rgb::RgbConverter;

pub struct Colors {
    pub rgb: ColorRgb,
    pub cmyk: ColorCmyk,
    pub picture: Rc<Picture>,

    pub red: Option<ScaleContainer>,
    pub green: Option<ScaleContainer>,
    pub blue: Option<ScaleContainer>,

    pub cyan: Option<ScaleContainer>,
    pub magenta: Option<ScaleContainer>,
    pub yellow: Option<ScaleContainer>,
    pub key: Option<ScaleContainer>,
}

impl Colors {
    pub(crate) fn new(picture: Picture) -> Colors {
        Colors {
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
            picture: Rc::new(picture),
            red: None,
            green: None,
            blue: None,
            cyan: None,
            magenta: None,
            yellow: None,
            key: None,
        }
    }
    fn rgb_changed(&mut self) -> (u8, u8, u8) {
        let cmyk = self.rgb.convert_cmyk();

        self.cyan.clone().unwrap().update(cmyk.0);
        self.magenta.clone().unwrap().update(cmyk.1);
        self.yellow.clone().unwrap().update(cmyk.2);
        self.key.clone().unwrap().update(cmyk.3);

        *self.cmyk.cyan.borrow_mut() = cmyk.0;
        *self.cmyk.magenta.borrow_mut() = cmyk.1;
        *self.cmyk.yellow.borrow_mut() = cmyk.2;
        *self.cmyk.key.borrow_mut() = cmyk.3;

        (*self.rgb.red.borrow_mut(), *self.rgb.green.borrow_mut(), *self.rgb.blue.borrow_mut())
    }
    fn cmyk_changed(&mut self) -> (u8, u8, u8) {
        let rgb = self.cmyk.convert_rgb();

        self.red.clone().unwrap().update(rgb.0);
        self.green.clone().unwrap().update(rgb.1);
        self.blue.clone().unwrap().update(rgb.2);

        *self.rgb.red.borrow_mut() = rgb.0;
        *self.rgb.green.borrow_mut() = rgb.1;
        *self.rgb.blue.borrow_mut() = rgb.2;

        rgb
    }
    pub fn update(&mut self, rgb_input: bool) {
        let (red, green, blue) = if rgb_input { self.rgb_changed() } else { self.cmyk_changed() };
        self.update_color(red, green, blue);
    }

    fn update_color(&mut self, red: u8, green: u8, blue: u8) {
        let pixbuf = Pixbuf::new(Colorspace::Rgb, false, 8, 1, 1);
        pixbuf.clone().unwrap().put_pixel(0, 0, red, green, blue, 0);
        self.picture.set_pixbuf(pixbuf.as_ref());
    }
}