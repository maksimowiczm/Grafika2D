use std::error::Error;
use opencv::{prelude::*};

pub trait Filters {
    fn mean_filter(&mut self, size: usize) -> Result<(), Box<dyn Error>>;
    fn sobel_filter(&mut self) -> Result<(), Box<dyn Error>>;
    fn high_pass_filter(&mut self, size: usize) -> Result<(), Box<dyn Error>>;
    fn gauss_filter(&mut self) -> Result<(), Box<dyn Error>>;
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
) {
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
                pixels[index] = (sums[channel] * multiplier) as u8;
            }
        }
    }
}

fn destruct_mat(mat: &mut Mat) -> Result<(usize, usize, usize, &mut [u8]), Box<dyn Error>> {
    let width = mat.size()?.width as usize;
    let height = mat.size()?.height as usize;
    let channels = mat.channels() as usize;
    let pixels = mat.data_bytes_mut()?;

    Ok((width, height, channels, pixels))
}

impl Filters for Mat {
    fn mean_filter(&mut self, size: usize) -> Result<(), Box<dyn Error>> {
        let avg = 1. / size.pow(2) as f64;
        let row = vec![avg; size];
        let mask = vec!(row; size);

        let (width, height, channels, pixels) = destruct_mat(self)?;
        mask_filter(pixels, width, height, channels, 1., mask);
        Ok(())
    }

    fn sobel_filter(&mut self) -> Result<(), Box<dyn Error>> {
        let mask = vec!(
            vec!(1., 0., -1.),
            vec!(2., 0., -2.),
            vec!(1., 0., -1.),
        );

        let (width, height, channels, pixels) = destruct_mat(self)?;
        mask_filter(pixels, width, height, channels, 1., mask);
        Ok(())
    }

    fn high_pass_filter(&mut self, size: usize) -> Result<(), Box<dyn Error>> {
        let mut mask = vec![vec![-1.; size]; size];
        mask[size / 2][size / 2] = size.pow(2) as f64;

        let (width, height, channels, pixels) = destruct_mat(self)?;
        mask_filter(pixels, width, height, channels, 1., mask);
        Ok(())
    }

    fn gauss_filter(&mut self) -> Result<(), Box<dyn Error>> {
        let mask = vec!(
            vec!(1., 4., 7., 4., 1.),
            vec!(4., 16., 26., 16., 4.),
            vec!(7., 26., 41., 26., 7.),
            vec!(4., 16., 26., 16., 4.),
            vec!(1., 4., 7., 4., 1.)
        );

        let (width, height, channels, pixels) = destruct_mat(self)?;
        mask_filter(pixels, width, height, channels, 1. / 273., mask);
        Ok(())
    }
}
