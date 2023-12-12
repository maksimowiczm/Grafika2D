import enum

import cv2
import numpy as np
import sys
import gi

from color_detector import ColorDetector

gi.require_version("Gtk", "4.0")
from gi.repository import Gtk, GdkPixbuf


class Scale(enum.Enum):
    Lower = 0,
    Upper = 1


class MyApplication(Gtk.Application):
    def __init__(self):
        super().__init__()
        self.bounds = [
            [0, 0, 0],
            [255, 255, 255]
        ]
        self.image: cv2.Mat = None
        self.start_image: cv2.Mat = None
        self.label = Gtk.Label.new()
        self.picture = Gtk.Picture.new()

    @staticmethod
    def image2pixbuf(image):
        height, width = image.shape[:2]
        img = np.reshape(image, (1, width * height * 3))[0]
        pixl = GdkPixbuf.Pixbuf.new_from_data(img.data, GdkPixbuf.Colorspace.RGB, False,
                                              8, width, height, width * 3, None)
        return pixl

    def load_image(self, path):
        image = cv2.imread(path)
        resized = cv2.resize(image, None, fx=0.5, fy=0.5)
        self.image = cv2.cvtColor(resized, cv2.COLOR_BGR2RGB)
        self.start_image = self.image.copy()
        pixbuf = MyApplication.image2pixbuf(self.image)
        self.picture.set_pixbuf(pixbuf)

    def update_pixbuf(self):
        lower = self.bounds[0]
        upper = self.bounds[1]

        detected = ColorDetector.detect_color(self.image, np.array(lower), np.array(upper))
        pixbuf = MyApplication.image2pixbuf(detected)
        self.picture.set_pixbuf(pixbuf)
        percent = np.sum(detected == 255) / detected.size
        self.label.set_label(f"{100 - percent * 100:.2f}%")

    def update_scale(self, _, __, value, scale: Scale, color: int):
        scale = 0 if scale == Scale.Lower else 1
        self.bounds[scale][color] = int(value)
        self.update_pixbuf()

    def build_scale(self, label: str, bound: Scale, color: int, default_value):
        adjustment = Gtk.Adjustment().new(default_value, 0, 255, 1, 0, 0)
        scale = Gtk.Scale.new(Gtk.Orientation.HORIZONTAL, adjustment)
        scale.connect("change-value", self.update_scale, bound, color)
        return scale

    def build_scales(self, scale: Scale, default_value):
        container = Gtk.Box.new(Gtk.Orientation.VERTICAL, 5)
        container.append(self.build_scale("red", scale, 0, default_value))
        container.append(self.build_scale("green", scale, 1, default_value))
        container.append(self.build_scale("blue", scale, 2, default_value))
        return container

    def dilate(self, _):
        dilated = cv2.erode(self.image, np.ones((3, 3), np.uint8), iterations=1)
        # dilated = cv2.dilate(self.image, np.ones((5, 5), np.uint8), iterations=1)
        self.image = dilated
        self.update_pixbuf()

    def erode(self, _):
        eroded = cv2.dilate(self.image, np.ones((5, 5), np.uint8), iterations=1)
        # eroded = cv2.erode(self.image, np.ones((3, 3), np.uint8), iterations=1)
        self.image = eroded
        self.update_pixbuf()

    def build_dilate_button(self):
        button = Gtk.Button.new_with_label("Dilate")
        button.connect("clicked", self.dilate)
        return button

    def build_erode_button(self):
        button = Gtk.Button.new_with_label("Erode")
        button.connect("clicked", self.erode)
        return button

    def detect_green(self, _):
        self.image = ColorDetector.detect_green(self.image.copy())
        self.update_pixbuf()

    def build_detect_green(self):
        button = Gtk.Button.new_with_label("Detect green")
        button.connect("clicked", self.detect_green)
        return button

    def reset_image(self, _):
        self.bounds[0] = [0, 0, 0]
        self.bounds[1] = [255, 255, 255]
        self.image = self.start_image.copy()
        self.update_pixbuf()

    def build_reset_image(self):
        button = Gtk.Button.new_with_label("Reset image")
        button.connect("clicked", self.reset_image)
        return button

    def do_activate(self):
        main_container = Gtk.Box.new(Gtk.Orientation.VERTICAL, 0)
        main_container.append(Gtk.Label.new("Lower bound"))
        main_container.append(self.build_scales(Scale.Lower, 0))
        main_container.append(Gtk.Label.new("Upper bound"))
        main_container.append(self.build_scales(Scale.Upper, 255))
        main_container.append(self.picture)
        main_container.append(self.label)
        main_container.append(self.build_detect_green())
        main_container.append(self.build_dilate_button())
        main_container.append(self.build_erode_button())
        main_container.append(self.build_reset_image())
        self.label.set_label(f"{100:.2f}%")

        path = "/home/user/Downloads/kampus-PB-analiza-terenow-zielonych.png"
        self.load_image(path)

        window = Gtk.ApplicationWindow(application=self, title="Hello World")
        window.set_child(main_container)
        window.present()


app = MyApplication()
exit_status = app.run(sys.argv)
sys.exit(exit_status)
