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
}

impl Default for DrawingContext {
    fn default() -> Self {
        DrawingContext {
            polygons: vec![Figure::default()],
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
    key_controller.connect_key_pressed(move |_, _, key, _| match key {
        65 => {
            // space - draw next shape
            if let Some(polygon) = context.borrow().polygons.last() {
                if let None = polygon.get_vertexes() {
                    return gtk::glib::Propagation::Stop;
                }
            }
            context.borrow_mut().polygons.push(Figure::default());
            area.queue_draw();
            gtk::glib::Propagation::Stop
        }
        27 => {
            // r - reset context
            *context.borrow_mut() = Default::default();
            area.queue_draw();
            gtk::glib::Propagation::Stop
        }
        _ => gtk::glib::Propagation::Stop,
    });
    parent_window.add_controller(key_controller);

    container
}
