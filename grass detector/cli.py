import cv2
import numpy as np

from color_detector import ColorDetector

path = "/home/user/Downloads/kampus-PB-analiza-terenow-zielonych.png"
image = cv2.imread(path)
img = cv2.resize(image, None, fx=0.5, fy=0.5)
cv2.imshow("img", img)

green = ColorDetector.detect_green(img)
cv2.imshow("green", green)

cv2.waitKey()
