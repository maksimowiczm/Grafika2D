use opencv::prelude::{*};
use crate::image::Image;


pub trait Histogram {
    fn get_histogram(&mut self) -> Vec<Vec<&mut u8>>;
}

impl Histogram for Mat {
    fn get_histogram(&mut self) -> Vec<Vec<&mut u8>> {
        let (_, _, _, pixels) = self.destruct_mut_mat().unwrap();

        let mut histogram: Vec<Vec<&mut u8>> = Vec::new();
        for _ in 0..256 {
            histogram.push(Vec::new());
        }

        pixels
            .iter_mut()
            .for_each(|pixel| {
                let val = *pixel;
                histogram[val as usize].push(pixel);
            });

        histogram
    }
}