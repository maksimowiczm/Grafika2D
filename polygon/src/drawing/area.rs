use crate::polygon::vertex::point::Point;
use std::{cell::RefCell, rc::Rc};

use core::option::Option;
use gtk::{prelude::DrawingAreaExtManual, traits::WidgetExt};

use crate::polygon::Polygon;

use super::{angle, drawing, scale, DrawingContext};

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
        if let Some(index) = borrowed.selected {
            match &mut borrowed.action {
                super::Action::Move { from, to, info } => {
                    let vec = (to.0 - from.0, to.1 - from.1);
                    let figure = &mut borrowed.polygons[index];
                    figure.move_polygon(&vec);
                }
                super::Action::Rotate { from, info, to } => {
                    info.clone().set_text("Drawing");
                    let reference = (from.0, from.1).into();
                    let angle = super::angle(*from, *to);
                    let figure = &mut borrowed.polygons[index];
                    figure.rotate(reference, angle);
                }
                super::Action::Scale { from, to, info } => {
                    info.clone().set_text("Drawing");
                    let reference = (from.0, from.1).into();
                    let scale = scale(*from, *to);
                    let figure = &mut borrowed.polygons[index];
                    figure.scale(reference, scale);
                }
                _ => (),
            }

            borrowed.selected = None;
            borrowed.action = super::Action::None;
            area.queue_draw();
            return;
        }

        if let Some(polygon) = &mut borrowed.polygons.last_mut() {
            polygon
                .add_vertex(Point::from((x as i16, y as i16)))
                .unwrap();
            area.queue_draw();
        }
    });

    gesture
}

fn build_right_click(
    area: gtk::DrawingArea,
    context: Rc<RefCell<DrawingContext>>,
) -> gtk::GestureClick {
    let gesture = gtk::GestureClick::builder().button(3).build();

    gesture.connect_pressed(move |_, _, x, y| {
        let mut borrowed = context.borrow_mut();
        if borrowed.selected.is_some() {
            borrowed.selected = None;
            borrowed.action = super::Action::None;
        }

        if let Some(_) = borrowed.polygons.first() {
            let closest = borrowed
                .polygons
                .iter()
                .enumerate()
                .map(
                    |(i, p)| match p.distance(Point::from((x as i16, y as i16))) {
                        Some(distance) => Some((i, distance)),
                        None => None,
                    },
                )
                .flatten()
                .fold::<Option<(usize, f64)>, _>(None, |acc, (i, distance)| match acc {
                    Some((_, current)) => {
                        if distance < current {
                            Some((i, distance))
                        } else {
                            acc
                        }
                    }
                    None => Some((i, distance)),
                });

            if let Some((index, distance)) = closest {
                if distance < 10. {
                    borrowed.selected = Some(index);
                }
            }
        };
        area.queue_draw();
    });

    gesture
}

fn build_drawing_area_motion(
    area: gtk::DrawingArea,
    context: Rc<RefCell<DrawingContext>>,
) -> gtk::EventControllerMotion {
    let motion_gesture = gtk::EventControllerMotion::new();

    motion_gesture.connect_motion(move |_, x, y| {
        let borrowed = &mut context.borrow_mut();

        match &mut borrowed.action {
            super::Action::Move { from, to, info, .. } => {
                *to = (x as i16, y as i16);
                info.set_text(format!("{:?} -> {:?}", from, to).as_str());
            }
            super::Action::Rotate { from, to, info, .. } => {
                *to = (x as i16, y as i16);
                let angle = angle(*from, *to);
                info.set_text(format!("angle = {:?}", angle).as_str());
            }
            super::Action::Scale { from, to, info, .. } => {
                *to = (x as i16, y as i16);
                let scale = scale(*from, *to);
                info.set_text(format!("scale = {:?}", scale).as_str());
            }
            _ => (),
        }

        area.queue_draw();
    });

    motion_gesture
}

pub fn build_area(context: Rc<RefCell<DrawingContext>>) -> gtk::DrawingArea {
    let area = gtk::DrawingArea::builder()
        .vexpand(true)
        .hexpand(true)
        .build();

    area.add_controller(build_left_click(area.clone(), Rc::clone(&context)));
    area.add_controller(build_right_click(area.clone(), Rc::clone(&context)));
    area.add_controller(build_drawing_area_motion(area.clone(), Rc::clone(&context)));
    area.set_draw_func(move |area, cr, w, h| draw(area, cr, w, h, Rc::clone(&context)));

    area
}
