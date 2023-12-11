#[allow(dead_code)]
pub const CHANNEL_RED: usize = 2;
#[allow(dead_code)]
pub const CHANNEL_GREEN: usize = 1;
#[allow(dead_code)]
pub const CHANNEL_BLUE: usize = 0;

pub trait Channels<T> {
    fn get_channel(&mut self, channels: usize, channel: usize) -> Box<dyn Iterator<Item=&mut u8> + '_>;
}

impl Channels<u8> for [u8] {
    fn get_channel(&mut self, channels: usize, channel: usize) -> Box<dyn Iterator<Item=&mut u8> + '_> {
        Box::new(self
            .into_iter()
            .enumerate()
            .filter(move |(index, _)| {
                *index % channels == channel
            })
            .map(|(_, b)| {
                b
            }))
    }
}
