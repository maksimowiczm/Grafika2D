mod area;
mod drawing;

use std::cell::RefCell;
use std::f64::consts::PI;
use std::io::prelude::*;
use std::rc::Rc;

use gtk::prelude::*;
use gtk::traits::BoxExt;

use crate::polygon::figure::Figure;
use crate::polygon::vertex::Vertex;
use crate::polygon::Polygon;

use self::area::build_area;

pub enum Action {
    None,
    Move {
        from: (i16, i16),
        to: (i16, i16),
        info: gtk::Label,
    },
    Rotate {
        from: (i16, i16),
        to: (i16, i16),
        info: gtk::Label,
    },
    Scale {
        from: (i16, i16),
        to: (i16, i16),
        info: gtk::Label,
    },
}

fn distance(from: (i16, i16), to: (i16, i16)) -> f64 {
    (((from.0 - to.0) as f64).powi(2) + ((from.1 - to.1) as f64).powi(2)).sqrt()
}

pub fn scale(from: (i16, i16), to: (i16, i16)) -> f64 {
    distance(from, to) / 100.
}

pub fn angle(from: (i16, i16), to: (i16, i16)) -> f64 {
    let arccos = (distance(from, (to.0, from.1)) / distance(from, to)).acos();
    let x = to.0 - from.0;
    let y = to.1 - from.1;
    let angle = if x > 0 && y > 0 {
        arccos
    } else if x < 0 && y > 0 {
        PI - arccos
    } else if x < 0 && y < 0 {
        PI + arccos
    } else {
        PI * 2. - arccos
    };

    angle
}

pub struct DrawingContext {
    polygons: Vec<Figure<i16>>,
    selected: Option<usize>,
    action: Action,
}

impl Default for DrawingContext {
    fn default() -> Self {
        DrawingContext {
            polygons: vec![Figure::default()],
            selected: None,
            action: Action::None,
        }
    }
}

pub fn build_drawing_area_container(parent_window: &gtk::ApplicationWindow) -> gtk::Box {
    let container = gtk::Box::new(gtk::Orientation::Vertical, 0);
    let context = Rc::new(RefCell::new(DrawingContext::default()));
    let info = gtk::Label::builder().label("Drawing").build();
    container.append(&info);
    let area = build_area(Rc::clone(&context));
    container.append(&area);

    let key_controller = gtk::EventControllerKey::new();
    key_controller.connect_key_pressed(move |_, key, _, _| match key {
        gtk::gdk::Key::space => {
            if let Some(polygon) = context.borrow().polygons.last() {
                if let None = context.borrow().selected {
                    info.set_text("Drawing");
                }
                if let Some(vertexes) = polygon.get_vertexes() {
                    if vertexes.is_empty() {
                        return gtk::glib::Propagation::Stop;
                    }
                } else {
                    return gtk::glib::Propagation::Stop;
                }
            }
            context.borrow_mut().polygons.push(Figure::default());
            area.queue_draw();
            gtk::glib::Propagation::Stop
        }
        gtk::gdk::Key::Escape => {
            *context.borrow_mut() = Default::default();
            area.queue_draw();
            gtk::glib::Propagation::Stop
        }
        gtk::gdk::Key::m => {
            let mut borrowed = context.borrow_mut();
            if let Some(index) = borrowed.selected {
                info.set_text("Moving");
                let figure = &mut borrowed.polygons[index];
                let (&x, &y) = figure.get_vertex(0).unwrap().get_coordinates();
                borrowed.action = Action::Move {
                    from: (x, y),
                    to: (x, y),
                    info: info.clone(),
                };
                area.queue_draw();
            }
            gtk::glib::Propagation::Stop
        }
        gtk::gdk::Key::r => {
            let mut borrowed = context.borrow_mut();
            if let Some(index) = borrowed.selected {
                info.set_text("Rotating");
                let figure = &mut borrowed.polygons[index];
                let (&x, &y) = figure.get_vertex(0).unwrap().get_coordinates();
                borrowed.action = Action::Rotate {
                    from: (x, y),
                    to: (x, y),
                    info: info.clone(),
                };
                area.queue_draw();
            }
            gtk::glib::Propagation::Stop
        }
        gtk::gdk::Key::s => {
            let mut borrowed = context.borrow_mut();
            if let Some(index) = borrowed.selected {
                info.set_text("Scaling");
                let figure = &mut borrowed.polygons[index];
                let (&x, &y) = figure.get_vertex(0).unwrap().get_coordinates();
                borrowed.action = Action::Scale {
                    from: (x, y),
                    to: (x, y),
                    info: info.clone(),
                };
                area.queue_draw();
            }
            gtk::glib::Propagation::Stop
        }
        gtk::gdk::Key::w => {
            let dialog = gtk::FileChooserDialog::builder()
                .action(gtk::FileChooserAction::Save)
                .build();
            dialog.add_button("Save", gtk::ResponseType::Ok);
            dialog.add_button("Cancel", gtk::ResponseType::Cancel);
            dialog.show();

            let c_context = Rc::clone(&context);
            dialog.connect_response(move |dialog, res| match res {
                gtk::ResponseType::Ok => {
                    dialog.close();
                    let file = dialog.file().unwrap();
                    let path = file.path().unwrap().to_str().unwrap().to_owned();
                    let mut file = std::fs::File::create(path).unwrap();
                    let json = serde_json::to_string(&c_context.borrow().polygons).unwrap();
                    file.write_all(json.as_bytes()).unwrap();
                }
                _ => (),
            });

            gtk::glib::Propagation::Stop
        }
        gtk::gdk::Key::l => {
            let dialog = gtk::FileChooserDialog::builder()
                .action(gtk::FileChooserAction::Open)
                .build();
            dialog.add_button("Load", gtk::ResponseType::Ok);
            dialog.add_button("Cancel", gtk::ResponseType::Cancel);
            dialog.show();

            let c_context = Rc::clone(&context);
            let c_area = area.clone();
            dialog.connect_response(move |dialog, res| match res {
                gtk::ResponseType::Ok => {
                    dialog.close();
                    let file = dialog.file().unwrap();
                    let path = file.path().unwrap().to_str().unwrap().to_owned();
                    let mut file = std::fs::File::open(path).unwrap();
                    let mut json = String::new();
                    file.read_to_string(&mut json).unwrap();
                    let polygons = serde_json::from_str::<Vec<Figure<i16>>>(json.as_str()).unwrap();
                    c_context.borrow_mut().polygons = polygons;
                    c_area.queue_draw();
                }
                _ => (),
            });

            gtk::glib::Propagation::Stop
        }
        _ => gtk::glib::Propagation::Stop,
    });
    parent_window.add_controller(key_controller);

    container
}
