use crate::polygon::vertex::point::Point;
use std::{cell::RefCell, f64::consts::PI, rc::Rc};

use gtk::{prelude::DrawingAreaExtManual, traits::WidgetExt};

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
    polygons.iter().for_each(|vertexes| {
        // draw point if figure has one vertex
        if vertexes.len() == 1 {
            let (x, y) = vertexes[0].get_coordinates();
            cr.arc(*x as f64, *y as f64, 1.5, 0., 2. * PI);
            cr.stroke().unwrap();
            return;
        }

        // draw edges
        vertexes.iter().for_each(|vertex| {
            let (x, y) = vertex.get_coordinates();
            cr.line_to(*x as f64, *y as f64)
        });

        // close polygon
        if let Some(first) = vertexes.first() {
            let (x, y) = first.get_coordinates();
            cr.line_to(*x as f64, *y as f64);
        }

        cr.stroke().unwrap();
    });
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
