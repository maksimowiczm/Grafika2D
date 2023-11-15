use num::integer::binomial;
use std::ops;

#[derive(Debug)]
pub struct Point {
    pub x: i32,
    pub y: i32,
}

pub trait BezierPoint<T> {
    fn bezier(&self, n: usize, i: usize, t: f64) -> Option<Point>;
    fn set_base(&self, base: &T) -> T;
}

impl BezierPoint<Point> for Point {
    fn bezier(&self, n: usize, i: usize, t: f64) -> Option<Point> {
        let bezier_self = |v: i32| {
            binomial(n as i128, i as i128) as f64
                * (1. - t).powf((n - i) as f64)
                * t.powf(i as f64)
                * v as f64
        };

        Some(Point::from((bezier_self(self.x), bezier_self(self.y))))
    }

    fn set_base(&self, base: &Point) -> Point {
        Point::from((self.x - base.x, self.y - base.y))
    }
}

impl ops::Add<&Point> for Point {
    type Output = Point;

    fn add(self, rhs: &Point) -> Self::Output {
        Point::from((self.x + rhs.x, self.y + rhs.y))
    }
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
