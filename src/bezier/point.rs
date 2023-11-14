#[derive(Debug)]
pub struct Point {
    pub x: i32,
    pub y: i32,
}

impl From<(i32, i32)> for Point {
    fn from(value: (i32, i32)) -> Self {
        Point {
            x: value.0,
            y: value.1,
        }
    }
}

impl From<(f64, f64)> for Point {
    fn from(value: (f64, f64)) -> Self {
        Point {
            x: value.0 as i32,
            y: value.1 as i32,
        }
    }
}

impl Default for Point {
    fn default() -> Self {
        Point { x: 0, y: 0 }
    }
}
