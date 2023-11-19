pub mod figure;
pub mod vertex;

use crate::polygon::vertex::Vertex;
use std::ops::AddAssign;

pub trait Polygon<TVertex, TCoord>
where
    TVertex: Vertex<TCoord>,
    TCoord: AddAssign,
{
    fn is_point(&self) -> bool;
    fn get_vertexes(&self) -> Option<&[TVertex]>;
    fn get_lines(&self) -> Vec<(&TVertex, &TVertex)>;
    fn add_vertex(&mut self, vertex: TVertex) -> Result<(), Box<dyn std::error::Error>>;
    fn get_vertex(&mut self, index: usize) -> Option<&mut TVertex>;
    fn rotate(&mut self, angle: f64);
    fn scale(&mut self, reference: (TCoord, TCoord), scale: f64);
    fn move_polygon(&mut self, vector: &(TCoord, TCoord));
}
