from typing import Any

import cv2
import numpy as np


class ColorDetector:
    @staticmethod
    def detect_green(image):
        # Define range of green color in HSV
        ranges = [
            (np.array([30, 30, 25]), np.array([100, 255, 255])),
            (np.array([0, 0, 0]), np.array([180, 100, 100])),
        ]

        return ColorDetector.detect_color_with_masks(image, ranges)

    @staticmethod
    def detect_color_with_masks(image, bounds: list[tuple[np.ndarray[Any, np.dtype], np.ndarray[Any, np.dtype]]]):
        img_hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
        masks = []
        for lower, upper in bounds:
            masks.append(cv2.inRange(img_hsv, lower, upper))

        result_mask: cv2.Mat = np.zeros_like(masks[0])
        for mask in masks:
            result_mask = cv2.bitwise_or(result_mask, mask)

        green = cv2.bitwise_and(image, image, mask=result_mask)
        green[green == 0] = 255

        return green

    @staticmethod
    def detect_color(image, lower_range, upper_range):
        # Convert the image to HSV color space
        img_hsv = cv2.cvtColor(image, cv2.COLOR_RGB2HSV)

        # Threshold the HSV image to get only green color
        mask = cv2.inRange(img_hsv, lower_range, upper_range)

        # Bitwise-AND mask and original image
        green = cv2.bitwise_and(image, image, mask=mask)

        # Replace black with white
        green[green == 0] = 255

        return green
