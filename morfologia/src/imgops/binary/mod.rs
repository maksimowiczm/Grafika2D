use opencv::core::Mat;
use opencv::{self as cv};

pub trait ImageBinary {
    fn load(path: &str) -> Result<Self, cv::Error>
    where
        Self: Sized;

    fn to_rgb(img: &Self) -> Result<Self, cv::Error>
    where
        Self: Sized;
}

impl ImageBinary for Mat {
    fn load(path: &str) -> Result<Self, cv::Error> {
        let img = cv::imgcodecs::imread(path, cv::imgcodecs::IMREAD_COLOR)?;
        let mut out = Mat::default();
        cv::imgproc::cvt_color(&img, &mut out, cv::imgproc::COLOR_RGB2GRAY, 0)?;
        let mut binarized = Mat::default();
        cv::imgproc::threshold(&out, &mut binarized, 200., 255., cv::imgproc::THRESH_BINARY)?;

        Ok(binarized)
    }

    fn to_rgb(img: &Self) -> Result<Self, cv::Error> {
        let mut out = Mat::default();
        cv::imgproc::cvt_color(&img, &mut out, cv::imgproc::COLOR_GRAY2RGB, 0)?;
        Ok(out)
    }
}
