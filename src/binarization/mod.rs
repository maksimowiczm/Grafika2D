use std::error::Error;
use opencv::core::Mat;
use opencv::prelude::*;
use crate::histogram::Histogram;
use crate::operations::Operations;

pub trait Binarization {
    fn threshold_binarization(&mut self, threshold: u8) -> Result<(), Box<dyn Error>>;
}

impl Binarization for Mat {
    fn threshold_binarization(&mut self, threshold: u8) -> Result<(), Box<dyn Error>> {
        self
            .gray_scale_mask()?;

        self
            .get_histogram()
            .iter_mut()
            .enumerate()
            .for_each(|(i, row)| {
                let value = if i < threshold as usize { 0 } else { 255 };
                row
                    .iter_mut()
                    .for_each(|pixel| **pixel = value);
            });

        Ok(())
    }
}