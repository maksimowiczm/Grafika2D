use opencv::{prelude::*};

trait Filters {}

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

impl Filters for Mat {}
