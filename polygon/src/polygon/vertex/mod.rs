pub mod point;

pub trait Vertex<T>
where
    T: std::ops::AddAssign,
{
    fn get_coordinates(&self) -> (&T, &T);
    fn move_vertex(&mut self, vector: (T, T)) -> Result<(), Box<dyn std::error::Error>>;
}
