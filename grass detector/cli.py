import cv2
import numpy as np

path = "/home/user/Downloads/kampus-PB-analiza-terenow-zielonych.png"
image = cv2.imread(path)
img = cv2.resize(image, None, fx=0.5, fy=0.5)

lower_range = np.array([30, 30, 30])
upper_range = np.array([100, 255, 255])

# Convert the image to HSV color space
img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

# Threshold the HSV image to get only green color
mask = cv2.inRange(img_hsv, lower_range, upper_range)

# Bitwise-AND mask and original image
green = cv2.bitwise_and(img, img, mask=mask)

# Replace black with white
green[green == 0] = 255

cv2.imshow("hsv", img_hsv)
cv2.imshow("mask", mask)
cv2.imshow("green", green)

cv2.waitKey()
