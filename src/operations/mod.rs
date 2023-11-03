use std::error::Error;
use opencv::prelude::*;
use crate::channels::Channels;
use crate::image::Image;
use crate::filters::get_pixel;

pub trait Operations {
    fn add_brightness(&mut self, value: i16) -> Result<(), Box<dyn Error>>;
    fn add_to_channel(&mut self, value: i16, channel: usize) -> Result<(), Box<dyn Error>>;
    fn gray_scale_mask(&mut self) -> Result<(), Box<dyn Error>>;
    fn gray_scale_average(&mut self) -> Result<(), Box<dyn Error>>;
}

fn no_overflow_add_u8(number: u8, to_add: i16) -> u8 {
    let mut new = number as i16 + to_add;
    if new > u8::MAX as i16 {
        new = u8::MAX as i16;
    } else if new < u8::MIN as i16 {
        new = u8::MIN as i16;
    }
    return new as u8;
}

impl Operations for Mat {
    fn add_brightness(&mut self, value: i16) -> Result<(), Box<dyn Error>> {
        self
            .data_bytes_mut()?
            .into_iter()
            .for_each(|b| *b = no_overflow_add_u8(*b, value));

        Ok(())
    }

    fn add_to_channel(&mut self, value: i16, channel: usize) -> Result<(), Box<dyn Error>> {
        let channels = self.channels();
        self
            .data_bytes_mut()?
            .get_channel(channels as usize, channel)
            .for_each(|b| *b = no_overflow_add_u8(*b, value));

        Ok(())
    }

    fn gray_scale_mask(&mut self) -> Result<(), Box<dyn Error>> {
        if self.channels() != 3 {
            return Err("bruh".into());
        }

        let (width, height, _, pixels) = self.destruct_mut_mat()?;

        for i in 0..height {
            for j in 0..width {
                let rgb = [
                    pixels[get_pixel(j, i, width, 3, 0)],
                    pixels[get_pixel(j, i, width, 3, 1)],
                    pixels[get_pixel(j, i, width, 3, 2)],
                ];

                let result = (0. +
                    0.299 * rgb[0] as f64 +
                    0.587 * rgb[1] as f64 +
                    0.114 * rgb[2] as f64
                ) as u8;

                pixels[get_pixel(j, i, width, 3, 0)] = result;
                pixels[get_pixel(j, i, width, 3, 1)] = result;
                pixels[get_pixel(j, i, width, 3, 2)] = result;
            }
        }

        Ok(())
    }

    fn gray_scale_average(&mut self) -> Result<(), Box<dyn Error>> {
        if self.channels() != 3 {
            return Err("bruh".into());
        }

        let (width, height, _, pixels) = self.destruct_mut_mat()?;

        for i in 0..height {
            for j in 0..width {
                let rgb = [
                    pixels[get_pixel(j, i, width, 3, 0)],
                    pixels[get_pixel(j, i, width, 3, 1)],
                    pixels[get_pixel(j, i, width, 3, 2)],
                ];

                let result = (rgb[0] + rgb[1] + rgb[2]) / 3;

                pixels[get_pixel(j, i, width, 3, 0)] = result;
                pixels[get_pixel(j, i, width, 3, 1)] = result;
                pixels[get_pixel(j, i, width, 3, 2)] = result;
            }
        }

        Ok(())
    }
}
