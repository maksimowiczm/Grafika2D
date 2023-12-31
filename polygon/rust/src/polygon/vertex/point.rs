use serde::{Deserialize, Serialize};
use crate::polygon::vertex::Vertex;

#[derive(Debug, Serialize, Deserialize)]
pub struct Point<T> {
    x: T,
    y: T,
}

impl<T> Vertex<T> for Point<T>
where
    T: std::ops::AddAssign + Copy,
{
    fn get_coordinates(&self) -> (&T, &T) {
        (&self.x, &self.y)
    }

    fn move_vertex(&mut self, vector: &(T, T)) {
        self.x += vector.0;
        self.y += vector.1;
    }

    fn set_coordinates(&mut self, coordinates: (T, T)) {
        self.x = coordinates.0;
        self.y = coordinates.1;
    }
}

impl Point<i16> {
    pub fn distance(&self, point: &Point<i16>) -> f64 {
        let (x, y) = point.get_coordinates();

        f64::sqrt(
            (self.x as f64 - *x as f64) * (self.x as f64 - *x as f64)
                + (self.y as f64 - *y as f64) * (self.y as f64 - *y as f64),
        )
    }
}

impl<T> From<(T, T)> for Point<T> {
    fn from(value: (T, T)) -> Self {
        Point {
            x: value.0,
            y: value.1,
        }
    }
}
