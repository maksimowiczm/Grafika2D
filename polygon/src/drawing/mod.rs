mod area;

use std::cell::RefCell;
use std::rc::Rc;

use gtk::traits::BoxExt;

use crate::polygon::figure::Figure;

use self::area::build_area;

pub struct DrawingContext {
    polygons: Vec<Figure<u16>>,
}

pub fn build_drawing_area_container() -> gtk::Box {
    let container = gtk::Box::new(gtk::Orientation::Horizontal, 0);
    let context = Rc::new(RefCell::new(DrawingContext { polygons: vec![] }));
    context.borrow_mut().polygons.push(Figure::default());
    container.append(&build_area(Rc::clone(&context)));
    container
}
