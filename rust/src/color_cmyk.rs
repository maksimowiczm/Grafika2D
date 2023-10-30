use std::cell::RefCell;
use std::rc::Rc;
use crate::color::Color;
use crate::color_rgb::{RgbConverter};

#[derive(Debug)]
pub struct ColorCmyk {
    pub cyan: Rc<RefCell<u8>>,
    pub magenta: Rc<RefCell<u8>>,
    pub yellow: Rc<RefCell<u8>>,
    pub key: Rc<RefCell<u8>>,
}

pub trait CmykConverter {
    fn convert_cmyk(&self) -> (u8, u8, u8, u8);
}

impl Color for ColorCmyk {
    fn max_value() -> u8 {
        100
    }
}

impl RgbConverter for ColorCmyk {
    fn convert_rgb(&self) -> (u8, u8, u8) {
        let cyan = *self.cyan.borrow() as f64 / 100.;
        let yellow = *self.yellow.borrow() as f64 / 100.;
        let magenta = *self.magenta.borrow() as f64 / 100.;
        let key = *self.key.borrow() as f64 / 100.;

        (
            (255. * (1. - cyan) * (1. - key)) as u8,
            (255. * (1. - magenta) * (1. - key)) as u8,
            (255. * (1. - yellow) * (1. - key)) as u8,
        )
    }
}