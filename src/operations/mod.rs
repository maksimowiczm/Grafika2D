use std::error::Error;
use opencv::prelude::*;
use crate::channels::Channels;

pub trait Operations {
    fn add_brightness(&mut self, value: i16) -> Result<(), Box<dyn Error>>;
    fn add_to_channel(&mut self, value: i16, channel: usize) -> Result<(), Box<dyn Error>>;
}

fn no_overflow_add_u8(number: u8, to_add: i16) -> u8 {
    let mut new = number as i16 + to_add;
    if new > u8::MAX as i16 {
        new = u8::MAX as i16;
    } else if new < u8::MIN as i16 {
        new = u8::MIN as i16;
    }
    return new as u8;
}

impl Operations for Mat {
    fn add_brightness(&mut self, value: i16) -> Result<(), Box<dyn Error>> {
        self
            .data_bytes_mut()?
            .into_iter()
            .for_each(|b| *b = no_overflow_add_u8(*b, value));

        Ok(())
    }

    fn add_to_channel(&mut self, value: i16, channel: usize) -> Result<(), Box<dyn Error>> {
        let channels = self.channels();
        self
            .data_bytes_mut()?
            .get_channel(channels as usize, channel)
            .for_each(|b| *b = no_overflow_add_u8(*b, value));

        Ok(())
    }
}
