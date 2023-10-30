use gtk::glib;
use gtk::subclass::prelude::*;

// Object holding the state
#[derive(Default)]
pub struct ScaleContainer {}

// The central trait for subclassing a GObject
#[glib::object_subclass]
impl ObjectSubclass for ScaleContainer {
    const NAME: &'static str = "MyGtkAppScaleBox";
    type Type = super::ScaleContainer;
    type ParentType = gtk::Box;
}

// Trait shared by all GObjects
impl ObjectImpl for ScaleContainer {}

// Trait shared by all widgets
impl WidgetImpl for ScaleContainer {}

// Trait shared by all buttons
impl BoxImpl for ScaleContainer {}