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
    T: AddAssign + Copy,
{
    fn is_point(&self) -> bool {
        self.points.len() == 1
    }

    fn get_vertexes(&self) -> Option<&[Point<T>]> {
        match self.points.as_slice() {
            [] => None,
            _ => Some(self.points.as_slice()),
        }
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

    fn move_polygon(&mut self, vector: &(T, T)) {
        self.points.iter_mut().for_each(|p| {
            p.move_vertex(vector);
        })
    }
}

impl Figure<i16> {
    pub(crate) fn distance(&self, point: Point<i16>) -> Option<f64> {
        if self.points.is_empty() {
            None
        } else {
            Some(
                self.points
                    .iter()
                    .map(|p| p.distance(&point))
                    .fold(f64::MAX, |acc, v| if v < acc { v } else { acc }),
            )
        }
    }

    pub fn rotate(&mut self, reference: Point<i16>, angle: f64) {
        let (&rx, &ry) = reference.get_coordinates();
        let sin = angle.sin();
        let cos = angle.cos();

        let mut new_points: Vec<_> = self
            .points
            .iter_mut()
            .map(|point| {
                let (&px, &py) = point.get_coordinates();

                let x = (rx + (px - rx)) as f64 * cos - (py - ry) as f64 * sin;
                let y = (ry + (px - rx)) as f64 * sin + (py - ry) as f64 * cos;

                (point, (x, y))
            })
            .collect();

        // make sure it fits inside area
        if new_points.iter().fold(
            false,
            |acc, (_, (x, y))| {
                if *x < 0. || *y < 0. {
                    true
                } else {
                    acc
                }
            },
        ) {
            return;
        }

        new_points
            .iter_mut()
            .for_each(|(point, (x, y))| point.set_coordinates((*x as i16, *y as i16)));
    }

    pub fn scale(&mut self, reference: Point<i16>, scale: f64) {
        let (&rx, &ry) = reference.get_coordinates();

        let mut new_points: Vec<_> = self
            .points
            .iter_mut()
            .map(|point| {
                let (&px, &py) = point.get_coordinates();

                let x = (rx + (px - rx)) as f64 * scale;
                let y = (ry + (py - ry)) as f64 * scale;

                (point, (x, y))
            })
            .collect();

        // make sure it fits inside area
        if new_points.iter().fold(
            false,
            |acc, (_, (x, y))| {
                if *x < 0. || *y < 0. {
                    true
                } else {
                    acc
                }
            },
        ) {
            return;
        }

        new_points
            .iter_mut()
            .for_each(|(point, (x, y))| point.set_coordinates((*x as i16, *y as i16)));
    }
}

impl<T> Default for Figure<T> {
    fn default() -> Self {
        Figure { points: vec![] }
    }
}
