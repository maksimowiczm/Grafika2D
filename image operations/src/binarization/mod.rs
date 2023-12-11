use std::error::Error;
use opencv::core::Mat;
use opencv::prelude::*;
use crate::histogram::{Histogram};
use crate::operations::{no_overflow_add_u8, Operations};

pub trait Binarization {
    fn threshold_binarization(&mut self, threshold: u8) -> Result<(), Box<dyn Error>>;
    fn percent_black_selection(&mut self, percent: f64) -> Result<u8, Box<dyn Error>>;
    fn mean_iterative_selection(&mut self) -> Result<u8, Box<dyn Error>>;
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
    fn percent_black_selection(&mut self, percent: f64) -> Result<u8, Box<dyn Error>> {
        if percent > 1. {
            return Err("Not a percent 💀".into());
        }

        let total = self.total() * self.channels() as usize;
        let mut sum = 0;

        let threshold = self
            .get_histogram()
            .iter_mut()
            .map(|row| {
                sum += row.len();
                let current = sum as f64 / total as f64;
                if current < percent {
                    Some(current)
                } else {
                    None
                }
            })
            .flatten()
            .collect::<Vec<_>>()
            .len();

        Ok(no_overflow_add_u8(threshold as u8, 1))
    }

    fn mean_iterative_selection(&mut self) -> Result<u8, Box<dyn Error>> {
        let mut threshold = self.percent_black_selection(0.25).unwrap() as usize;
        let histogram = self.get_histogram();

        loop {
            let (tbu, tbd) = histogram
                .iter()
                .enumerate()
                .filter(|(i, _)| *i < threshold)
                .fold((0, 0), |(u, d), (i, row)| (u + i * row.len(), d + row.len()));
            let tb = tbu as f64 / tbd as f64;

            let (twu, twd) = histogram
                .iter()
                .enumerate()
                .filter(|(i, _)| *i >= threshold)
                .fold((0, 0), |(u, d), (i, row)| (u + i * row.len(), d + row.len()));
            let tw = twu as f64 / twd as f64;

            let prev_threshold = threshold;
            threshold = ((tb + tw) / 2.) as usize;

            if threshold == prev_threshold {
                break;
            }
        }

        Ok(threshold as u8)
    }
}