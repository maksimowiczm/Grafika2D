pub mod point;

use crate::bezier::point::Point;

#[derive(Debug)]
pub struct Bezier {
    points: Vec<Point>,
}

impl Default for Bezier {
    fn default() -> Self {
        Bezier { points: vec![] }
    }
}

impl Bezier {
    pub(crate) fn add_point(&mut self, point: Point) {
        self.points.push(point)
    }
}
