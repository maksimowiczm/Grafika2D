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
    cr.set_source_rgb(255., 255., 255.);

    // draw shapes
    context
        .borrow()
        .polygons
        .iter()
        .map(|polygon| polygon.get_lines())
        .for_each(|lines| {
            if let Some((first, _)) = lines.first() {
                let (x, y) = first.get_coordinates();
                cr.line_to(*x as f64, *y as f64);
            }

            lines.iter().for_each(|(_, to)| {
                let (x, y) = to.get_coordinates();
                cr.line_to(*x as f64, *y as f64);
            });

            cr.stroke().unwrap();
        });

    // draw points
    context
        .borrow()
        .polygons
        .iter()
        .filter(|polygon| polygon.is_point())
        .map(|polygon| polygon.get_vertexes())
        .flatten()
        .map(|vertexes| vertexes.first())
        .flatten()
        .for_each(|point| {
            let (x, y) = point.get_coordinates();
            cr.arc(*x as f64, *y as f64, 1.5, 0., 2. * PI);
            cr.stroke().unwrap();
        })
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
