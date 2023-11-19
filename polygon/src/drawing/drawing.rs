use crate::drawing::DrawingContext;
use crate::polygon::figure::Figure;
use crate::polygon::vertex::Vertex;
use crate::polygon::Polygon;
use std::f64::consts::PI;

fn draw_shapes<'a>(
    cr: &gtk::cairo::Context,
    iter: impl IntoIterator<Item = &'a Figure<u16>>,
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
    iter: impl IntoIterator<Item = &'a Figure<u16>>,
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
    iter: impl IntoIterator<Item = &'a Figure<u16>> + Clone,
    color: (f64, f64, f64),
) {
    draw_shapes(cr, iter.clone(), color);
    draw_points(cr, iter, color);
}

pub(crate) fn draw_context(context: &DrawingContext, cr: &gtk::cairo::Context) {
    let iter = context.polygons.iter().rev();

    // draw current
    draw_polygons(cr, iter.clone().take(1), (255., 0., 0.));

    // draw rest
    draw_polygons(cr, iter.clone().skip(1), (255., 255., 255.));
}
