import Augment
import cv2
import numpy as np


'''
The code is for OpenCV format.
If your data format is PIL.Image, please convert the format by:

import numpy as np
import cv2
from PIL import Image

img = Image.open("The Path to the image")
img = cv2.cvtColor(np.asarray(img), cv2.COLOR_RGB2BGR)
'''

im = cv2.imread("pic/demo.png")
im = cv2.resize(im, (200, 64))
cv2.imshow("im_CV", im)
for i in range(5000):
	im_Distort = Augment.GenerateDistort(im, 4)
	cv2.imshow("im_Distort", im_Distort)
	im_Stretch = Augment.GenerateStretch(im, 4)
	cv2.imshow("im_Stretch", im_Stretch)
	im_Perspective = Augment.GeneratePerspective(im)
	cv2.imshow("im_Perspective", im_Perspective)
	cv2.waitKey(1)
