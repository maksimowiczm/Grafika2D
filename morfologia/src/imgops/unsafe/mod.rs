mod r#impl;

use crate::imgops::HitOrMiss;

#[link(name = "imgops", kind = "static")]
extern "C" {
    fn imgops_hit_or_miss(
        image: *const u8,
        image_width: u64,
        image_height: u64,
        x: u64,
        y: u64,
        mask: *const u8,
        mask_width: u64,
        mask_height: u64,
    ) -> i8;
}

pub unsafe trait UnsafeHitOrMissMask {
    unsafe fn hit_or_miss(&self, pixel: (usize, usize), mask: &Vec<Vec<u8>>) -> HitOrMiss;

    unsafe fn apply_mask(&mut self, mask: &Vec<Vec<u8>>, value: u8);

    unsafe fn dilatation(&mut self, mask: &Vec<Vec<u8>>);

    unsafe fn erosion(&mut self, mask: &Vec<Vec<u8>>);

    unsafe fn closing(&mut self, mask: &Vec<Vec<u8>>);

    unsafe fn opening(&mut self, mask: &Vec<Vec<u8>>);
}
