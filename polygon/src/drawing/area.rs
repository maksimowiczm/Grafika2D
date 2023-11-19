use crate::polygon::vertex::point::Point;
use std::{cell::RefCell, rc::Rc};

use gtk::{prelude::DrawingAreaExtManual, traits::WidgetExt};

use crate::polygon::Polygon;

use super::{drawing, DrawingContext};

fn draw(
    _: &gtk::DrawingArea,
    cr: &gtk::cairo::Context,
    _: i32,
    _: i32,
    context: Rc<RefCell<DrawingContext>>,
) {
    drawing::draw_context(&context.borrow(), cr)
}

fn build_left_click(
    area: gtk::DrawingArea,
    context: Rc<RefCell<DrawingContext>>,
) -> gtk::GestureClick {
    let gesture = gtk::GestureClick::builder().button(1).build();

    gesture.connect_pressed(move |_, _, x, y| {
        let borrowed = &mut context.borrow_mut();
        if let Some(polygon) = &mut borrowed.polygons.last_mut() {
            polygon
                .add_vertex(Point::from((x as u16, y as u16)))
                .unwrap();
            area.queue_draw();
        }
    });

    gesture
}

pub fn build_area(context: Rc<RefCell<DrawingContext>>) -> gtk::DrawingArea {
    let area = gtk::DrawingArea::builder()
        .vexpand(true)
        .hexpand(true)
        .build();

    area.add_controller(build_left_click(area.clone(), Rc::clone(&context)));
    area.set_draw_func(move |area, cr, w, h| draw(area, cr, w, h, Rc::clone(&context)));

    area
}
