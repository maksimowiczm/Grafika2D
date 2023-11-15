use crate::bezier::BezierCurve;
use gtk::{prelude::*, DrawingArea};
use std::cell::RefCell;
use std::rc::Rc;

fn build_drawing_area_gesture(
    bezier: Rc<RefCell<BezierCurve>>,
    area: DrawingArea,
) -> gtk::GestureClick {
    let gesture = gtk::GestureClick::builder().button(1).build();
    gesture.connect_pressed(move |_, _, x, y| {
        bezier.borrow_mut().add_point(From::from((x, y)));
        area.queue_draw();
    });

    gesture
}

pub fn build_drawing_area(bezier: Rc<RefCell<BezierCurve>>) -> gtk::DrawingArea {
    let drawing_area = gtk::DrawingArea::builder()
        .height_request(500)
        .width_request(500)
        .build();

    drawing_area.add_controller(build_drawing_area_gesture(
        Rc::clone(&bezier),
        drawing_area.clone(),
    ));

    drawing_area
        .set_draw_func(move |area, cr, w, h| draw_function(area, cr, w, h, Rc::clone(&bezier)));

    drawing_area
}

fn draw_function(
    _: &DrawingArea,
    cr: &gtk::cairo::Context,
    _: i32,
    _: i32,
    bezier: Rc<RefCell<BezierCurve>>,
) {
    let accuracy = 100;
    let points: Vec<_> = (0..accuracy + 1)
        .map(|x| x as f64 / accuracy as f64)
        .map(|i| bezier.borrow().bezier(i))
        .flatten()
        .collect();

    if let Some(first) = points.first() {
        cr.set_source_rgb(255., 255., 255.);
        cr.move_to(first.x as f64, first.y as f64);
        points
            .iter()
            .for_each(|point| cr.line_to(point.x as f64, point.y as f64));

        cr.stroke().unwrap();
    }
}
