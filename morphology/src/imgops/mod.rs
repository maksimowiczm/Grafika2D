pub mod binary;
mod r#impl;
pub mod r#unsafe;

#[derive(Debug)]
pub enum HitOrMiss {
    ERROR,
    MISS,
    FIT,
    HIT,
}

pub trait HitOrMissMask {
    fn hit_or_miss(&self, pixel: (usize, usize), mask: &Vec<Vec<u8>>) -> HitOrMiss;

    fn apply_mask(&mut self, mask: &Vec<Vec<u8>>, value: u8);

    fn dilatation(&mut self, mask: &Vec<Vec<u8>>);

    fn erosion(&mut self, mask: &Vec<Vec<u8>>);

    fn closing(&mut self, mask: &Vec<Vec<u8>>);

    fn opening(&mut self, mask: &Vec<Vec<u8>>);
}
