use opencv::{prelude::*};

use gtk::{glib};
use gtk::gdk_pixbuf::Pixbuf;
use glib::Bytes;

use crate::image::Image;

pub struct Context {
    pub(crate) starting: Option<Mat>,
    pub(crate) mat: Option<Mat>,
}

pub fn picture_update_pixbuf(picture: &gtk::Picture, mat: &Mat) {
    let (width, height, channels, pixels) = mat.destruct_mat().unwrap();
    let bytes = Bytes::from(&pixels);
    let pixbuf = Pixbuf::from_bytes(&bytes,
                                    gtk::gdk_pixbuf::Colorspace::Rgb,
                                    false,
                                    8,
                                    width as i32,
                                    height as i32,
                                    width as i32 * channels as i32);

    picture.set_pixbuf(Some(pixbuf.as_ref()));
}