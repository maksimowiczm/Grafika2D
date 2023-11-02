use std::error::Error;
use opencv::{prelude::*};
use crate::image::Image;

pub trait Filters {
    fn mean_filter(&mut self, size: usize) -> Result<(), Box<dyn Error>>;
    fn median_filter(&mut self, size: usize) -> Result<(), Box<dyn Error>>;
    fn sobel_filter(&mut self) -> Result<(), Box<dyn Error>>;
    fn high_pass_filter(&mut self, size: usize) -> Result<(), Box<dyn Error>>;
    fn gauss_filter(&mut self) -> Result<(), Box<dyn Error>>;
    fn mask_filter(&mut self, multiplier: f64, mask: Vec<Vec<f64>>) -> Result<(), Box<dyn Error>>;
}

fn get_pixel(x: usize, y: usize, width: usize, channels: usize, channel: usize) -> usize {
    width * y * channels + x * channels + channel
}

fn mask_filter(pixels: &mut [u8],
               width: usize,
               height: usize,
               channels: usize,
               multiplier: f64,
               mask: Vec<Vec<f64>>,
) -> Result<(), Box<dyn Error>> {
    let mask_height = mask.len();
    let mask_width = mask.first().unwrap().len();

    let start_x = mask_width / 2;
    let start_y = mask_height / 2;

    let pixels_length = pixels.len();
    let mut image = vec![0; pixels_length];
    image[..pixels_length].clone_from_slice(pixels);

    let mut sums: Vec<f64>;

    for i in start_y..height - start_y {
        for j in start_x..width - start_x {
            sums = vec![0.; channels];

            for y in 0..mask_height {
                for x in 0..mask_width {
                    let mask_value = mask[y][x];
                    for channel in 0..channels {
                        let index = get_pixel(j - start_x + x, i - start_y + y, width, channels, channel);
                        let pixel = image[index];
                        sums[channel] += pixel as f64 * mask_value;
                    }
                }
            }

            for channel in 0..channels {
                let index = get_pixel(j, i, width, channels, channel);
                let mut sum = sums[channel] * multiplier;
                if sum > u8::MAX as f64 {
                    sum = u8::MAX as f64;
                }
                pixels[index] = sum as u8;
            }
        }
    }

    Ok(())
}

fn method_filter(pixels: &mut [u8],
                 width: usize,
                 height: usize,
                 channels: usize,
                 filter_height: usize,
                 filter_width: usize,
                 filter_method: impl Fn(Vec<u8>) -> u8,
) -> Result<(), Box<dyn Error>> {
    let start_x = filter_width / 2;
    let start_y = filter_height / 2;

    let pixels_length = pixels.len();
    let mut image = vec![0; pixels_length];
    image[..pixels_length].clone_from_slice(pixels);


    for i in start_y..height - start_y {
        for j in start_x..width - start_x {
            for channel in 0..channels {
                let mut to_filter = Vec::new();

                for y in 0..filter_height {
                    for x in 0..filter_width {
                        let index = get_pixel(j - start_x + x, i - start_y + y, width, channels, channel);
                        let pixel = image[index];
                        to_filter.push(pixel);
                    }
                }

                let index = get_pixel(j, i, width, channels, channel);
                pixels[index] = filter_method(to_filter);
            }
        }
    }

    Ok(())
}


impl Filters for Mat {
    fn mean_filter(&mut self, size: usize) -> Result<(), Box<dyn Error>> {
        let avg = 1. / size.pow(2) as f64;
        let row = vec![avg; size];
        let mask = vec!(row; size);

        self.mask_filter(1., mask)
    }

    fn median_filter(&mut self, size: usize) -> Result<(), Box<dyn Error>> {
        let c_size = size * size;
        let method = |pixels: Vec<u8>| {
            let mut sorted = pixels.clone();
            sorted.sort();
            sorted[c_size / 2]
        };

        let (width, height, channels, pixels) = self.destruct_mut_mat()?;
        method_filter(pixels, width, height, channels, size, size, method)
    }

    fn sobel_filter(&mut self) -> Result<(), Box<dyn Error>> {
        let mask = vec!(
            vec!(1., 0., -1.),
            vec!(2., 0., -2.),
            vec!(1., 0., -1.),
        );

        self.mask_filter(1., mask)
    }

    fn high_pass_filter(&mut self, size: usize) -> Result<(), Box<dyn Error>> {
        let mut mask = vec![vec![-1.; size]; size];
        mask[size / 2][size / 2] = size.pow(2) as f64;

        self.mask_filter(1., mask)
    }

    fn gauss_filter(&mut self) -> Result<(), Box<dyn Error>> {
        let mask = vec!(
            vec!(1., 4., 7., 4., 1.),
            vec!(4., 16., 26., 16., 4.),
            vec!(7., 26., 41., 26., 7.),
            vec!(4., 16., 26., 16., 4.),
            vec!(1., 4., 7., 4., 1.)
        );

        self.mask_filter(1. / 273., mask)
    }

    fn mask_filter(&mut self, multiplier: f64, mask: Vec<Vec<f64>>) -> Result<(), Box<dyn Error>> {
        let (width, height, channels, pixels) = self.destruct_mut_mat()?;
        mask_filter(pixels, width, height, channels, multiplier, mask)
    }
}
