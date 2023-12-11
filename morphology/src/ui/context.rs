use crate::imgops::binary::ImageBinary;
use opencv::prelude::*;

use glib::Bytes;
use gtk::gdk_pixbuf::Pixbuf;
use gtk::glib;

pub struct Context {
    pub starting: Option<Mat>,
    pub mat: Option<Mat>,
}

impl Default for Context {
    fn default() -> Self {
        Context {
            starting: None,
            mat: None,
        }
    }
}

pub fn picture_update_pixbuf(picture: &gtk::Picture, mat: &Mat) {
    let rgb = mat.to_rgb().unwrap();
    let size = rgb.size().unwrap();
    let (width, height) = (size.width, size.height);
    let pixels = rgb.data_bytes().unwrap();
    let bytes = Bytes::from(&pixels);

    let pixbuf = Pixbuf::from_bytes(
        &bytes,
        gtk::gdk_pixbuf::Colorspace::Rgb,
        false,
        8,
        width,
        height,
        width * 3,
    );

    picture.set_pixbuf(Some(pixbuf.as_ref()));
}
