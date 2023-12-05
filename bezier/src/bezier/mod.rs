use crate::bezier::point::BezierPoint;
pub mod point;

use crate::bezier::point::Point;

#[derive(Debug)]
pub struct BezierCurve {
    pub selected: Option<usize>,
    pub points: Vec<Point>,
}

impl Default for BezierCurve {
    fn default() -> Self {
        BezierCurve {
            points: vec![],
            selected: None,
        }
    }
}

impl BezierCurve {
    pub(crate) fn add_point(&mut self, point: Point) {
        self.points.push(point)
    }

    pub(crate) fn bezier(&self, t: f64) -> Option<Point> {
        if t < 0. || t > 1. {
            return None;
        }

        let length = self.points.len();

        if length < 2 {
            return None;
        }

        let first = self.points.first()?;

        let res = self.points.iter().enumerate().filter(|(i, _)| *i > 0).fold(
            Point::default(),
            |current_point, (i, point)| {
                current_point + &point.set_base(first).bezier(length - 1, i - 1, t).unwrap()
            },
        );

        Some(res + first)
    }
}
