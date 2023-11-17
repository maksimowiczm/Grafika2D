use crate::polygon::vertex::Vertex;

#[derive(Debug)]
pub struct Point<T> {
    x: T,
    y: T,
}

impl<T> Vertex<T> for Point<T>
where
    T: std::ops::AddAssign,
{
    fn get_coordinates(&self) -> (&T, &T) {
        (&self.x, &self.y)
    }

    fn move_vertex(&mut self, vector: (T, T)) -> Result<(), Box<dyn std::error::Error>> {
        self.x += vector.0;
        self.y += vector.1;

        Ok(())
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
