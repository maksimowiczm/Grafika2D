mod area;
mod drawing;

use std::cell::RefCell;
use std::rc::Rc;

use gtk::prelude::*;
use gtk::traits::BoxExt;

use crate::polygon::figure::Figure;
use crate::polygon::vertex::Vertex;
use crate::polygon::Polygon;

use self::area::build_area;

pub enum Action {
    None,
    Move { from: (i16, i16), to: (i16, i16) },
    Rotate { from: (i16, i16) },
    Scale { from: (i16, i16) },
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
    let container = gtk::Box::new(gtk::Orientation::Horizontal, 0);
    let context = Rc::new(RefCell::new(DrawingContext::default()));
    let area = build_area(Rc::clone(&context));
    container.append(&area);

    let key_controller = gtk::EventControllerKey::new();
    key_controller.connect_key_pressed(move |_, key, _, _| match key {
        gtk::gdk::Key::space => {
            if let Some(polygon) = context.borrow().polygons.last() {
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
                let figure = &mut borrowed.polygons[index];
                let (&x, &y) = figure.get_vertex(0).unwrap().get_coordinates();
                borrowed.action = Action::Move {
                    from: (x, y),
                    to: (x, y),
                };
                area.queue_draw();
            }
            gtk::glib::Propagation::Stop
        }
        _ => gtk::glib::Propagation::Stop,
    });
    parent_window.add_controller(key_controller);

    container
}
