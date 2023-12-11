use crate::imgops::{HitOrMiss, HitOrMissMask};
use opencv::core::Mat;

use opencv::prelude::*;

impl HitOrMissMask for Mat {
    fn hit_or_miss(&self, (x, y): (usize, usize), mask: &Vec<Vec<u8>>) -> HitOrMiss {
        let size = self.size().unwrap();
        if y + mask.len() > size.width as usize || x + mask[0].len() > size.height as usize {
            return HitOrMiss::ERROR;
        }

        let mask_height = mask.len();
        let mask_width = mask[0].len();

        let mut hits = 0; // count how many hits pixel hit
        let mut mask_times = 0; // count how many trues in mask
        for j in 0..mask_height {
            for i in 0..mask_width {
                let mask_value = mask[j][i];

                if mask_value == 0 {
                    continue;
                }

                mask_times += 1;
                match self.at_2d::<u8>((y + j) as i32, (x + i) as i32) {
                    Ok(pixel) => {
                        if *pixel == 0 {
                            hits += 1
                        }
                    }
                    _ => (),
                }
            }
        }

        if hits == 0 {
            HitOrMiss::MISS
        } else if hits == mask_times {
            HitOrMiss::FIT
        } else {
            HitOrMiss::HIT
        }
    }

    fn apply_mask(&mut self, mask: &Vec<Vec<u8>>, value: u8) {
        let size = self.size().unwrap();

        let hits = (0..size.height)
            .flat_map(|y| {
                let image = &self;
                let mask = &mask;
                (0..size.width)
                    .flat_map(
                        move |x| match image.hit_or_miss((x as usize, y as usize), mask) {
                            HitOrMiss::HIT => Some((x, y)),
                            _ => None,
                        },
                    )
                    .collect::<Vec<_>>()
            })
            .collect::<Vec<_>>();

        hits.iter().for_each(|&(x, y)| {
            *self.at_2d_mut(y, x).unwrap() = value;
        });
    }

    fn dilatation(&mut self, mask: &Vec<Vec<u8>>) {
        self.apply_mask(mask, 255)
    }

    fn erosion(&mut self, mask: &Vec<Vec<u8>>) {
        self.apply_mask(mask, 0)
    }

    fn closing(&mut self, mask: &Vec<Vec<u8>>) {
        self.dilatation(mask);
        self.erosion(mask);
    }

    fn opening(&mut self, mask: &Vec<Vec<u8>>) {
        self.erosion(mask);
        self.dilatation(mask);
    }
}
