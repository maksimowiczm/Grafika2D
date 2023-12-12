import cv2
import numpy as np

from color_detector import ColorDetector

path = "/home/user/Downloads/kampus-PB-analiza-terenow-zielonych.png"
image = cv2.imread(path)
img = cv2.resize(image, None, fx=0.5, fy=0.5)
cv2.imshow("img", img)

ranges = [
    (np.array([30, 30, 25]), np.array([100, 255, 255])),
    (np.array([0, 0, 0]), np.array([180, 100, 100])),
]

green = ColorDetector.detect_green(img)
cv2.imshow("green", green)

cv2.waitKey()
