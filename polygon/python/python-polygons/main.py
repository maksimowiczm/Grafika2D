import math
import json
import sys
from enum import Enum
from typing import List

import gi
import numpy

gi.require_version("Gtk", "4.0")
from gi.repository import GLib, Gtk, Gdk


def angle(from_coords, to):
    def distance(from_coords, to):
        from_x, from_y = from_coords
        to_x, to_y = to
        return math.sqrt((to_x - from_x) ** 2 + (to_y - from_y) ** 2)

    from_x, from_y = from_coords
    to_x, to_y = to
    arccos = math.acos(distance(from_coords, (to_x, from_y)) / distance(from_coords, to))
    x = to_x - from_x
    y = to_y - from_y
    if x > 0 and y > 0:
        angle = arccos
    elif x < 0 and y > 0:
        angle = math.pi - arccos
    elif x < 0 and y < 0:
        angle = math.pi + arccos
    else:
        angle = 2 * math.pi - arccos

    return angle


class Point:
    def __init__(self, x, y):
        self.coord = numpy.array((x, y))

    def to_json(self):
        return [int(self.x()), int(self.y())]

    def x(self):
        return self.coord[0]

    def y(self):
        return self.coord[1]

    def distance(self, x, y) -> float:
        return numpy.linalg.norm(self.coord - numpy.array((x, y)))

    def move(self, x, y):
        self.coord = numpy.add(self.coord, numpy.array((x, y)))

    def rotate(self, angle, reference):
        x = reference[0]
        y = reference[1]
        pointMatrix = numpy.array([self.x() - x, self.y() - y, 1]).reshape(3, 1)
        translationMatrix = (
            numpy.array([numpy.cos(angle), -numpy.sin(angle), 0, numpy.sin(angle), numpy.cos(angle), 0, 0, 0, 1])
            .reshape(3, 3)
        )

        result = numpy.matmul(translationMatrix, pointMatrix)
        self.coord = numpy.array((x + result[0, 0], y + result[1, 0]))

    def scale(self, scale, reference):
        x = reference[0]
        y = reference[1]

        pointMatrix = numpy.array([self.x() - x, self.y() - y, 1]).reshape(3, 1)
        translationMatrix = (
            numpy.array([scale, 0, 0, 0, scale, 0, 0, 0, 1])
            .reshape(3, 3)
        )

        result = numpy.matmul(translationMatrix, pointMatrix)
        self.coord = numpy.array((x + result[0, 0], y + result[1, 0]))


class Polygon:
    def __init__(self, points):
        self.points: List[Point] = points

    def distance(self, point: Point) -> (float, int):
        min = 100000.
        min_i = 123123
        for i, vertex in enumerate(self.points):
            distance = vertex.distance(point.x(), point.y())
            if distance < min:
                min = distance
                min_i = i

        return min, min_i

    def move(self, x, y):
        for point in self.points:
            point.move(x, y)

    def rotate(self, angle, reference):
        for point in self.points:
            point.rotate(angle, reference)

    def scale(self, scale, reference):
        for point in self.points:
            point.scale(scale, reference)

    def to_json(self):
        points = []
        for point in self.points:
            points.append(point.to_json())
        return points


class State(Enum):
    Drawing = 1,
    Moving = 2,
    Rotating_Point = 3,
    Rotating_Angle = 4,
    Scaling_Point = 5,
    Scaling_Scale = 6,


class MyApplication(Gtk.Application):
    def __init__(self):
        super().__init__(application_id="com.example.MyGtkApplication")
        self.area = None
        self.selected = None
        self.state = State.Drawing
        self.reference = None
        self.cursor = 0, 0
        GLib.set_application_name('My Gtk Application')
        points: List[Point] = []
        self.polygons: List[Polygon] = [Polygon(points)]

    def build_area(self, label):
        area = Gtk.DrawingArea.new()
        area.set_vexpand(True)
        area.set_hexpand(True)

        def area_draw(___, cr, _, __):
            for i, polygon in enumerate(self.polygons):
                if self.selected is not None and i == self.selected[0]:
                    cr.set_source_rgb(255, 0, 0)
                else:
                    cr.set_source_rgb(0, 0, 0)

                if len(polygon.points) == 0:
                    continue

                if len(polygon.points) == 1:
                    point = polygon.points[0]
                    cr.arc(point.x(), point.y(), 1.5, 0, 2 * math.pi)
                    cr.stroke()
                    continue

                cr.move_to(polygon.points[0].x(), polygon.points[0].y())
                for j, point in enumerate(polygon.points):
                    if j == 0:
                        continue
                    cr.line_to(point.x(), point.y())
                cr.line_to(polygon.points[0].x(), polygon.points[0].y())

                cr.stroke()

            if self.selected is not None:
                cr.set_source_rgb(0, 255, 0)
                point = self.polygons[self.selected[0]].points[self.selected[1]]
                cr.arc(point.x(), point.y(), 1.5, 0, 2 * math.pi)
                cr.stroke()

            if self.state == State.Moving:
                cr.set_source_rgb(0, 0, 255)
                point = self.polygons[self.selected[0]].points[self.selected[1]]
                cr.move_to(point.x(), point.y())
                cr.line_to(self.cursor[0], self.cursor[1])
                cr.stroke()
            elif self.state == State.Scaling_Scale:
                cr.set_source_rgb(0, 0, 255)
                cr.move_to(self.reference[0], self.reference[1])
                cr.line_to(self.cursor[0], self.cursor[1])
                cr.stroke()
            elif self.state == State.Rotating_Angle:
                cr.set_source_rgb(0, 0, 255)
                cr.move_to(10000, self.reference[1])
                cr.line_to(self.reference[0], self.reference[1])
                cr.line_to(self.cursor[0], self.cursor[1])
                cr.stroke()
                my_angle = angle(self.reference, self.cursor)
                cr.arc(self.reference[0], self.reference[1], 25, 0, my_angle)
                cr.stroke()
            elif self.state == State.Rotating_Point or self.state == State.Scaling_Point:
                cr.set_source_rgb(0, 0, 255)
                cr.arc(self.cursor[0], self.cursor[1], 1.5, 0, 2 * math.pi)
                cr.stroke()

        area.set_draw_func(area_draw)

        def left_click(_1, _, x, y):
            if self.state == State.Drawing:
                if self.selected is not None:
                    return
                last = len(self.polygons) - 1
                self.polygons[last].points.append(Point(x, y))
                area.queue_draw()
            elif self.state == State.Moving:
                if self.selected is None:
                    return
                polygon = self.polygons[self.selected[0]]
                point = polygon.points[self.selected[1]]
                polygon.move(-point.x() + self.cursor[0], -point.y() + self.cursor[1])
                area.queue_draw()
            elif self.state == State.Rotating_Point:
                self.reference = self.cursor
                self.state = State.Rotating_Angle
            elif self.state == State.Scaling_Point:
                self.reference = self.cursor
                self.state = State.Scaling_Scale
            elif self.state == State.Rotating_Angle:
                if self.selected is None:
                    return
                polygon = self.polygons[self.selected[0]]
                my_angle = angle(self.reference, self.cursor)
                polygon.rotate(my_angle, self.reference)
                area.queue_draw()
            elif self.state == State.Scaling_Scale:
                if self.selected is None:
                    return
                polygon = self.polygons[self.selected[0]]
                scale = numpy.linalg.norm(numpy.array(self.reference) - numpy.array(self.cursor)) / 100
                polygon.scale(scale, self.reference)
                area.queue_draw()

        gesture_left = Gtk.GestureClick.new()
        gesture_left.set_button(1)
        gesture_left.connect("pressed", left_click)
        area.add_controller(gesture_left)

        def right_click(_1, _, x, y):
            if self.state != State.Drawing:
                return

            self.selected = None

            min = 100000., -1, -1
            for i, polygon in enumerate(self.polygons):
                distance = polygon.distance(Point(x, y))
                if distance[0] < min[0]:
                    min = (distance[0], i, distance[1])

            if min[0] > 20:
                area.queue_draw()
                return

            self.selected = (min[1], min[2])
            area.queue_draw()

        gesture_right = Gtk.GestureClick.new()
        gesture_right.set_button(3)
        gesture_right.connect("pressed", right_click)
        area.add_controller(gesture_right)

        def motion(_, x, y):
            self.cursor = x, y
            area.queue_draw()
            if self.state == State.Drawing:
                label.set_text("Drawing")
            elif self.state == State.Moving:
                point = self.polygons[self.selected[0]].points[self.selected[1]]
                label.set_text(f"Moving ({round(-point.x() + self.cursor[0])}, {round(-point.y() + self.cursor[1])})")
            elif self.state == State.Rotating_Point or self.state == State.Scaling_Point:
                label.set_text(f"Reference: ({round(self.cursor[0])},{round(self.cursor[1])})")
            elif self.state == State.Rotating_Angle:
                my_angle = angle(self.reference, self.cursor)
                label.set_text(f"Rotating ({round(self.reference[0]), round(self.reference[1])}) ({my_angle} radians)")
            elif self.state == State.Scaling_Scale:
                scale = numpy.linalg.norm(numpy.array(self.reference) - numpy.array(self.cursor)) / 100
                label.set_text(f"Scaling ({scale})")

        gesture_motion = Gtk.EventControllerMotion.new()
        gesture_motion.connect("motion", motion)
        area.add_controller(gesture_motion)

        return area

    def do_activate(self):
        window = Gtk.ApplicationWindow(application=self, title="Hello World")
        label = Gtk.Label.new()
        label.set_text("drawing")
        area = self.build_area(label)

        def key_pressed(_, key, __, ___):
            if key == Gdk.KEY_space:
                self.polygons.append(Polygon([]))
                self.selected = None
            if key == Gdk.KEY_c:
                self.polygons = [Polygon([])]
                self.selected = None
            if key == Gdk.KEY_m and self.selected is not None:
                self.state = State.Moving
                label.set_text("moving")
            elif key == Gdk.KEY_Escape:
                self.state = State.Drawing
                self.selected = None
                label.set_text("drawing")
            elif key == Gdk.KEY_r and self.selected is not None:
                self.state = State.Rotating_Point
                label.set_text("rotating")
            elif key == Gdk.KEY_s and self.selected is not None:
                self.state = State.Scaling_Point
                label.set_text("scaling")
            elif key == Gdk.KEY_w:
                dialog = Gtk.FileChooserDialog()
                dialog.set_action(Gtk.FileChooserAction.SAVE)
                dialog.add_button("Save", Gtk.ResponseType.OK)
                dialog.add_button("Cancel", Gtk.ResponseType.CANCEL)

                def dialogResponse(dialog: Gtk.FileChooserDialog, res):
                    if res == Gtk.ResponseType.OK:
                        path = dialog.get_file().get_path()
                        with open(path, "w") as f:
                            polygons = []
                            for polygon in self.polygons:
                                polygons.append(polygon.to_json());
                            f.write(json.dumps(polygons))

                    dialog.destroy()

                dialog.connect("response", dialogResponse)
                dialog.show()
            elif key == Gdk.KEY_l:
                dialog = Gtk.FileChooserDialog()
                dialog.set_action(Gtk.FileChooserAction.OPEN)
                dialog.add_button("Load", Gtk.ResponseType.OK)
                dialog.add_button("Cancel", Gtk.ResponseType.CANCEL)

                def dialogResponse(dialog: Gtk.FileChooserDialog, res):
                    if res == Gtk.ResponseType.OK:
                        path = dialog.get_file().get_path()
                        with open(path, "r") as f:
                            polygons_json = f.read()
                            polygons = json.loads(polygons_json)
                            self.polygons = []
                            for polygon in polygons:
                                points = []
                                for point in polygon:
                                    points.append(Point(point[0], point[1]))
                                self.polygons.append(Polygon(points))

                    dialog.destroy()

                dialog.connect("response", dialogResponse)
                dialog.show()

            area.queue_draw()

        key_controller = Gtk.EventControllerKey.new()
        key_controller.connect("key-pressed", key_pressed)
        window.add_controller(key_controller)

        main_container = Gtk.Box.new(Gtk.Orientation.VERTICAL, 0)
        main_container.append(label)
        main_container.append(area)
        window.set_child(main_container)
        window.present()


app = MyApplication()
exit_status = app.run(sys.argv)
sys.exit(exit_status)
