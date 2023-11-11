use std::error::Error;
use opencv::prelude::{*};
use crate::image::Image;
use crate::operations::Operations;


pub trait Histogram {
    fn get_histogram(&mut self) -> Vec<Vec<&mut u8>>;
    fn histogram_normalize(&mut self) -> Result<(), Box<dyn Error>>;
    fn histogram_equalize(&mut self) -> Result<(), Box<dyn Error>>;
}

fn get_histogram_min_max(histogram: &Vec<Vec<&mut u8>>) -> (usize, usize) {
    let mut min = 0;
    while histogram[min].len() == 0 {
        min += 1;
    }
    let mut max = 255;
    while histogram[max].len() == 0 {
        max -= 1;
    }

    (min, max)
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
    fn histogram_normalize(&mut self) -> Result<(), Box<dyn Error>> {
        self.gray_scale_mask()?;

        let mut histogram = self.get_histogram();
        let (min, max) = get_histogram_min_max(&histogram);

        for i in min..max {
            let pos = (255. / (max as f64 - min as f64) * (i - min) as f64) as u8;

            histogram[i]
                .iter_mut()
                .for_each(|v| **v = pos);
        }

        Ok(())
    }
    fn histogram_equalize(&mut self) -> Result<(), Box<dyn Error>> {
        self.gray_scale_mask()?;

        let mut histogram = self.get_histogram();
        let mut sum = 0;
        let mut min_probability = usize::MAX;
        let cumulative: Vec<_> = histogram
            .iter_mut()
            .map(|row| {
                let len = row.len();

                min_probability = std::cmp::min(len, min_probability);
                if len < min_probability {
                    min_probability = len;
                }

                sum += len;
                sum
            })
            .collect();

        let (min, max) = get_histogram_min_max(&histogram);
        let length = *cumulative.last().unwrap();

        for i in min..max {
            let pos = (cumulative[i] as f64 - min_probability as f64) / (length as f64 - min_probability as f64) * u8::MAX as f64;
            histogram[i]
                .iter_mut()
                .for_each(|v| **v = pos as u8);
        }

        Ok(())
    }
}