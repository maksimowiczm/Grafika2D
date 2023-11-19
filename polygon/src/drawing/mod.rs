mod area;
mod drawing;

use std::cell::RefCell;
use std::rc::Rc;

use gtk::prelude::*;
use gtk::traits::BoxExt;

use crate::polygon::figure::Figure;
use crate::polygon::Polygon;

use self::area::build_area;

pub struct DrawingContext {
    polygons: Vec<Figure<u16>>,
    selected: Option<usize>,
}

impl Default for DrawingContext {
    fn default() -> Self {
        DrawingContext {
            polygons: vec![Figure::default()],
            selected: None,
        }
    }
}

pub fn build_drawing_area_container(parent_window: &gtk::ApplicationWindow) -> gtk::Box {
    let container = gtk::Box::new(gtk::Orientation::Horizontal, 0);
    let context = Rc::new(RefCell::new(DrawingContext::default()));
    context.borrow_mut().polygons.push(Figure::default());
    let area = build_area(Rc::clone(&context));
    container.append(&area);

    let key_controller = gtk::EventControllerKey::new();
    key_controller.connect_key_pressed(move |_, key, _, _| match key {
        gtk::gdk::Key::space => {
            if let Some(polygon) = context.borrow().polygons.last() {
                if let None = polygon.get_vertexes() {
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
        _ => gtk::glib::Propagation::Stop,
    });
    parent_window.add_controller(key_controller);

    container
}
