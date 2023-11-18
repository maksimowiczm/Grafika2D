use std::{cell::RefCell, rc::Rc};

use gtk::prelude::DrawingAreaExtManual;

use crate::polygon::{vertex::Vertex, Polygon};

use super::DrawingContext;

fn draw(
    _: &gtk::DrawingArea,
    cr: &gtk::cairo::Context,
    _: i32,
    _: i32,
    context: Rc<RefCell<DrawingContext>>,
) {
    let borrowed = context.borrow();
    let polygons: Vec<_> = borrowed
        .polygons
        .iter()
        .map(|polygon| polygon.get_vertexes())
        .flatten()
        .collect();

    // connect vertexes
    cr.set_source_rgb(255., 255., 255.);
    polygons.iter().for_each(|polygon| {
        polygon.iter().for_each(|vertex| {
            let (x, y) = vertex.get_coordinates();
            cr.line_to(*x as f64, *y as f64)
        });

        // close polygon
        if let Some(first) = polygon.first() {
            let (x, y) = first.get_coordinates();
            cr.line_to(*x as f64, *y as f64);
        }

        cr.stroke().unwrap();
    });
}

pub fn build_area(context: Rc<RefCell<DrawingContext>>) -> gtk::DrawingArea {
    let area = gtk::DrawingArea::builder()
        .vexpand(true)
        .hexpand(true)
        .build();

    area.set_draw_func(move |area, cr, w, h| draw(area, cr, w, h, Rc::clone(&context)));

    area
}
