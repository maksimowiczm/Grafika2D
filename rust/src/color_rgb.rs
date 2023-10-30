use std::cell::RefCell;
use std::rc::Rc;
use crate::color::Color;
use crate::color_cmyk::CmykConverter;

#[derive(Debug)]
pub struct ColorRgb {
    pub red: Rc<RefCell<u8>>,
    pub green: Rc<RefCell<u8>>,
    pub blue: Rc<RefCell<u8>>,
}

impl Color for ColorRgb {
    fn max_value() -> u8 {
        255
    }
}

pub trait RgbConverter {
    fn convert_rgb(&self) -> (u8, u8, u8);
}

impl CmykConverter for ColorRgb {
    fn convert_cmyk(&self) -> (u8, u8, u8, u8) {
        let red = *self.red.borrow() as f64 / 255.;
        let green = *self.green.borrow() as f64 / 255.;
        let blue = *self.blue.borrow() as f64 / 255.;
        let key = 1. - (*[*self.red.borrow(), *self.green.borrow(), *self.blue.borrow()].iter().max().unwrap() as f64 / 255.);
        if key == 1. {
            return (0, 0, 0, 100);
        }

        let cyan = (1. - red - key) / (1. - key);
        let magenta = (1. - green - key) / (1. - key);
        let yellow = (1. - blue - key) / (1. - key);

        (
            (cyan * 100.) as u8,
            (magenta * 100.) as u8,
            (yellow * 100.) as u8,
            (key * 100.) as u8,
        )
    }
}