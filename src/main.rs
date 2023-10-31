mod filters;
mod operations;
mod channels;
mod image;


use gtk::prelude::*;
use gtk::{glib, Application, ApplicationWindow};
use gtk::gdk_pixbuf::Colorspace::Rgb;
use gtk::gdk_pixbuf::Pixbuf;
use glib::Bytes;
use crate::image::Image;

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

    main_container.append(&build_actions_container(window.clone(), picture.clone()));
    main_container.append(&picture);

    window.present();
}

fn build_actions_container(window: ApplicationWindow, picture: gtk::Picture) -> gtk::Box {
    let actions_container = gtk::Box::new(gtk::Orientation::Vertical, 10);
    actions_container.append(&build_load_button(window.clone(), picture.clone()));
    actions_container
}

fn build_load_button(window: ApplicationWindow, picture: gtk::Picture) -> gtk::Button
{
    let button = gtk::Button::with_label("Load image");
    button.connect_clicked(move |_| build_file_chooser(window.clone(), picture.clone()).show());
    button
}

fn build_file_chooser(window: ApplicationWindow, picture: gtk::Picture) -> gtk::FileChooserDialog {
    let chooser = gtk::FileChooserDialog::new(
        Some("Load image"),
        Some(&window),
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
                    let (width, height, channels, pixels) = out.destruct_mat().unwrap();
                    let bytes = Bytes::from(&pixels);
                    let pixbuf = Pixbuf::from_bytes(&bytes,
                                                    Rgb,
                                                    false,
                                                    8,
                                                    width as i32,
                                                    height as i32,
                                                    width as i32 * channels as i32);
                    picture.set_pixbuf(Some(pixbuf.as_ref()));
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
