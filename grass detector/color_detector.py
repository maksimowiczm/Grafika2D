import cv2
import numpy as np


class ColorDetector:
    @staticmethod
    def detect_green(image):
        # Define range of green color in HSV
        lower_green = np.array([30, 30, 30])
        upper_green = np.array([100, 255, 255])

        return ColorDetector.detect_color(image, lower_green, upper_green)

    @staticmethod
    def detect_color(image, lower_range, upper_range):
        # Convert the image to HSV color space
        img_hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

        # Threshold the HSV image to get only green color
        mask = cv2.inRange(img_hsv, lower_range, upper_range)

        # Bitwise-AND mask and original image
        green = cv2.bitwise_and(image, image, mask=mask)

        # Replace black with white
        green[green == 0] = 255

        return green
