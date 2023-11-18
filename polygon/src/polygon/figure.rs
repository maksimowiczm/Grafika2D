use crate::polygon::vertex::point::Point;
use crate::polygon::vertex::Vertex;
use crate::polygon::Polygon;
use std::ops::AddAssign;

#[derive(Debug)]
pub struct Figure<T> {
    points: Vec<Point<T>>,
}

impl<T> Polygon<Point<T>, T> for Figure<T>
where
    Point<T>: Vertex<T>,
    T: AddAssign,
{
    fn get_vertexes(&self) -> Option<&[Point<T>]> {
        match self.points.as_slice() {
            [] => None,
            _ => Some(self.points.as_slice()),
        }
    }

    fn is_point(&self) -> bool {
        self.points.len() == 1
    }

    fn get_lines(&self) -> Vec<(&Point<T>, &Point<T>)> {
        if self.points.len() <= 1 {
            return vec![];
        }

        let line = (&self.points[0], &self.points[1]);
        let mut lines = self
            .points
            .iter()
            .skip(2)
            .fold(vec![line], |mut lines, point| {
                lines.push((lines.last().unwrap().1, &point));
                lines
            });

        if lines.len() > 1 {
            lines.push((&self.points.last().unwrap(), &self.points.first().unwrap()));
        }

        lines
    }

    fn add_vertex(&mut self, vertex: Point<T>) -> Result<(), Box<dyn std::error::Error>> {
        self.points.push(vertex);
        Ok(())
    }

    fn get_vertex(&mut self, index: usize) -> Option<&mut Point<T>> {
        self.points.get_mut(index)
    }

    fn rotate(&mut self, angle: f64) {
        todo!()
    }

    fn scale(&mut self, reference: (T, T), scale: f64) {
        todo!()
    }
}

impl<T> Default for Figure<T> {
    fn default() -> Self {
        Figure { points: vec![] }
    }
}
