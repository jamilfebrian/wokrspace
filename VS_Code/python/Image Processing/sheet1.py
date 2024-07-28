import cv2
import numpy as np

# Read the image
image = cv2.imread('path_to_your_image.jpg')

# Convert to grayscale
gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# Apply Gaussian blur
blurred_image = cv2.GaussianBlur(gray_image, (5, 5), 0)

# Display the images
cv2.imshow(image) # Original Image
cv2.imshow(gray_image) # Grayscale Image
cv2.imshow(blurred_image)
