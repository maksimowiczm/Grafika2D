import cv2
import numpy as np

from color_detector import ColorDetector

path = "/home/user/Downloads/kampus-PB-analiza-terenow-zielonych.png"

image = cv2.imread(path)
resized = cv2.resize(image, None, fx=0.5, fy=0.5)
cv2.imshow("image", resized)

green = ColorDetector.detect_green(resized)
cv2.imshow("greened", green)

percent = np.sum(green == 255) / green.size
print(percent)

cv2.waitKey()
