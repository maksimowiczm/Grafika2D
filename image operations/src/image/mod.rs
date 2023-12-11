use opencv::{self as cv, prelude::*};
use std::error::Error;

pub trait Image {
    fn destruct_mut_mat(&mut self) -> Result<(usize, usize, usize, &mut [u8]), Box<dyn Error>>;
    fn destruct_mat(&self) -> Result<(usize, usize, usize, &[u8]), Box<dyn Error>>;
}

impl Image for Mat {
    fn destruct_mut_mat(&mut self) -> Result<(usize, usize, usize, &mut [u8]), Box<dyn Error>> {
        let width = self.size()?.width as usize;
        let height = self.size()?.height as usize;
        let channels = self.channels() as usize;
        let pixels = self.data_bytes_mut()?;

        Ok((width, height, channels, pixels))
    }

    fn destruct_mat(&self) -> Result<(usize, usize, usize, &[u8]), Box<dyn Error>> {
        let width = self.size()?.width as usize;
        let height = self.size()?.height as usize;
        let channels = self.channels() as usize;
        let pixels = self.data_bytes()?;

        Ok((width, height, channels, pixels))
    }
}

pub fn load_image(path: &str) -> Result<Mat, Box<dyn Error>> {
    let img = cv::imgcodecs::imread(path, cv::imgcodecs::IMREAD_COLOR)?;

    let scale = (1280. / img.size()?.width as f64).min(720. / img.size()?.height as f64);
    let mut resized = Mat::default();
    cv::imgproc::resize(&img, &mut resized, opencv::core::Size::default(), scale, scale, cv::imgproc::INTER_AREA)?;

    let mut out = Mat::default();
    cv::imgproc::cvt_color(&resized, &mut out, cv::imgproc::COLOR_BGR2RGB, 0)?;

    Ok(out)
}