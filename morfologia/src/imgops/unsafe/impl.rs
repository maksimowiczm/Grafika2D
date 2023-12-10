use crate::imgops::r#unsafe::imgops_hit_or_miss;
use crate::imgops::r#unsafe::UnsafeHitOrMissMask;
use crate::imgops::HitOrMiss;
use opencv::core::Mat;
use opencv::prelude::*;

unsafe impl UnsafeHitOrMissMask for Mat {
    unsafe fn hit_or_miss(&self, (x, y): (usize, usize), mask: &Vec<Vec<u8>>) -> HitOrMiss {
        let size = self.size().unwrap();
        let data = self.data();
        let flat_mask = mask.iter().flatten().copied().collect::<Vec<_>>();
        let res = imgops_hit_or_miss(
            data,
            size.width as u64,
            size.height as u64,
            x as u64,
            y as u64,
            flat_mask.as_slice().as_ptr(),
            mask[0].len() as u64,
            mask.len() as u64,
        );
        match res {
            -1 => HitOrMiss::ERROR,
            0 => HitOrMiss::MISS,
            1 => HitOrMiss::HIT,
            2 => HitOrMiss::FIT,
            _ => panic!(),
        }
    }

    unsafe fn apply_mask(&mut self, mask: &Vec<Vec<u8>>, value: u8) {
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

    unsafe fn dilatation(&mut self, mask: &Vec<Vec<u8>>) {
        self.apply_mask(mask, 255)
    }

    unsafe fn erosion(&mut self, mask: &Vec<Vec<u8>>) {
        self.apply_mask(mask, 0)
    }

    unsafe fn closing(&mut self, mask: &Vec<Vec<u8>>) {
        self.dilatation(mask);
        self.erosion(mask);
    }

    unsafe fn opening(&mut self, mask: &Vec<Vec<u8>>) {
        self.erosion(mask);
        self.dilatation(mask);
    }
}
