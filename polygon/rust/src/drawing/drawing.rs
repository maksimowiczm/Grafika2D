use crate::drawing::DrawingContext;
use crate::polygon::figure::Figure;
use crate::polygon::vertex::Vertex;
use crate::polygon::Polygon;
use std::f64::consts::PI;

use super::angle;

fn draw_shapes<'a>(
    cr: &gtk::cairo::Context,
    iter: impl IntoIterator<Item = &'a Figure<i16>>,
    color: (f64, f64, f64),
) {
    cr.set_source_rgb(color.0, color.1, color.2);

    iter.into_iter()
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
}

fn draw_points<'a>(
    cr: &gtk::cairo::Context,
    iter: impl IntoIterator<Item = &'a Figure<i16>>,
    color: (f64, f64, f64),
) {
    cr.set_source_rgb(color.0, color.1, color.2);

    iter.into_iter()
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

fn draw_polygons<'a>(
    cr: &gtk::cairo::Context,
    iter: impl IntoIterator<Item = &'a Figure<i16>> + Clone,
    color: (f64, f64, f64),
) {
    draw_shapes(cr, iter.clone(), color);
    draw_points(cr, iter, color);
}

pub(crate) fn draw_context(context: &DrawingContext, cr: &gtk::cairo::Context) {
    if let Some((poly_index, _)) = context.selected {
        let selected = [&context.polygons[poly_index]];
        draw_polygons(cr, selected, (0., 0., 255.));

        draw_polygons(
            cr,
            context
                .polygons
                .iter()
                .enumerate()
                .filter(|(i, _)| *i != poly_index)
                .map(|(_, p)| p),
            (255., 255., 255.),
        );

        cr.set_source_rgb(0., 255., 0.);
        match &context.action {
            super::Action::Move { from, to, .. } | super::Action::Scale { from, to, .. } => {
                cr.line_to(from.0 as f64, from.1 as f64);
                cr.line_to(to.0 as f64, to.1 as f64);
                cr.stroke().unwrap();
            }
            super::Action::Rotate { from, to, .. } => {
                cr.line_to(from.0 as f64, from.1 as f64);
                cr.line_to(to.0 as f64, to.1 as f64);
                cr.stroke().unwrap();

                cr.line_to(from.0 as f64, from.1 as f64);
                cr.line_to(10000., from.1 as f64);
                cr.stroke().unwrap();

                let angle = angle(*from, *to);
                cr.arc(from.0 as f64, from.1 as f64, 15., 0., angle);
                cr.stroke().unwrap();
            }
            _ => (),
        }
    } else {
        let iter = context.polygons.iter().rev();
        draw_polygons(cr, iter.clone().take(1), (255., 0., 0.));
        draw_polygons(cr, iter.clone().skip(1), (255., 255., 255.));
    }
}
