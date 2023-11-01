mod filters;
mod operations;
mod channels;
mod image;

use opencv::{prelude::*};

use std::cell::RefCell;
use std::error::Error;
use std::rc::Rc;
use gtk::prelude::*;
use gtk::{glib, Application, ApplicationWindow};
use gtk::gdk_pixbuf::Pixbuf;
use glib::Bytes;
use crate::filters::Filters;
use crate::image::Image;

struct Context {
    starting: Option<Mat>,
    mat: Option<Mat>,
}

const APP_ID: &str = "org.grafika.rust.obrazy";

fn main() -> glib::ExitCode {
    let app = Application::builder().application_id(APP_ID).build();
    app.connect_activate(build_ui);
    app.run()
}

fn build_ui(app: &Application) {
    let main_container = gtk::Box::new(gtk::Orientation::Horizontal, 10);
    main_container.set_size_request(800, 600);

    let window = ApplicationWindow::builder()
        .application(app)
        .title("Images reader")
        .child(&main_container)
        .build();

    let picture = gtk::Picture::new();
    let context = Rc::new(RefCell::new(Context { starting: None, mat: None }));

    main_container.append(&build_actions_container(&window, picture.clone(), Rc::clone(&context)));
    main_container.append(&picture);

    window.present();
}

fn build_actions_container(window: &ApplicationWindow, picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Box {
    let actions_container = gtk::Box::new(gtk::Orientation::Vertical, 10);
    actions_container.append(&build_load_button(window.clone(), picture.clone(), Rc::clone(&context)));
    actions_container.append(&build_reset_button(picture.clone(), Rc::clone(&context)));
    actions_container.append(&build_filters_container(picture.clone(), Rc::clone(&context)));
    actions_container
}

fn build_reset_button(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Button {
    let button = gtk::Button::with_label("Reset image");
    button.connect_clicked(move |_| {
        let mut c_context = context.borrow_mut();
        if let Some(mat) = &c_context.starting {
            picture_update_pixbuf(&picture, &mat);
            c_context.mat = Some(mat.clone());
        }
    });
    button
}

fn build_load_button(window: ApplicationWindow, picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Button {
    let button = gtk::Button::with_label("Load image");
    button.connect_clicked(move |_| build_file_chooser(&window, picture.clone(), Rc::clone(&context)).show());
    button
}

fn build_file_chooser(window: &ApplicationWindow, picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::FileChooserDialog {
    let chooser = gtk::FileChooserDialog::new(
        Some("Load image"),
        Some(window),
        gtk::FileChooserAction::Open,
        &[("Open", gtk::ResponseType::Ok), ("Cancel", gtk::ResponseType::Cancel)],
    );

    chooser.connect_response(move |dialog, response| {
        if response != gtk::ResponseType::Ok {
            dialog.destroy();
        }

        let c_dialog = dialog.clone();
        let handle_error = || {
            let error = gtk::MessageDialog::new(Some(&c_dialog),
                                                gtk::DialogFlags::DESTROY_WITH_PARENT,
                                                gtk::MessageType::Error,
                                                gtk::ButtonsType::Ok,
                                                "Image format is not supported or file is corrupted!");
            error.connect_response(move |dialog, _| dialog.destroy());
            error.show();
        };

        // bro 💀
        if let Some(file) = dialog.file() {
            if let Some(path) = file.path() {
                if let Ok(out) = image::load_image(path.to_str().unwrap()) {
                    context.borrow_mut().mat = Some(out.clone());
                    context.borrow_mut().starting = Some(out.clone());
                    picture_update_pixbuf(&picture, &out);
                    dialog.destroy();
                } else {
                    handle_error();
                }
            } else {
                handle_error();
            }
        } else {
            handle_error();
        }
    });

    chooser
}

fn build_filters_container(picture: gtk::Picture, context: Rc<RefCell<Context>>) -> gtk::Box {
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

    filters_container
}

fn build_filter_button(label: &str, picture: gtk::Picture, context: Rc<RefCell<Context>>, method: fn(&mut Mat) -> Result<(), Box<dyn Error>>) -> gtk::Button {
    let button = gtk::Button::with_label(label);
    button.connect_clicked(move |_| {
        if let Some(mat) = &mut context.borrow_mut().mat {
            method(mat).unwrap();
            picture_update_pixbuf(&picture, &mat);
        }
    });
    button
}

fn picture_update_pixbuf(picture: &gtk::Picture, mat: &Mat) {
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
