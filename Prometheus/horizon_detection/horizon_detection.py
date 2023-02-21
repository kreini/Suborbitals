# import libraries
import numpy as np
import matplotlib.pyplot as plt
import cv2
import time

# init
cap = cv2.VideoCapture(0)
threshold = 1500
is_first = 1

while(True):

    # read image
    ret, img = cap.read()
    
    # get RGB channels
    blue, green, red = cv2.split(img)
    zeros = np.zeros(blue.shape, np.uint8)

    blueBGR = cv2.merge((zeros, zeros, blue))
    greenBGR = cv2.merge((zeros, green, zeros))
    redBGR = cv2.merge((red, zeros, zeros))

    # gaussian smoothing
    blueBGR_g = cv2.GaussianBlur(blueBGR, (5, 5), cv2.BORDER_DEFAULT)
    greenBGR_g = cv2.GaussianBlur(greenBGR, (5, 5), cv2.BORDER_DEFAULT)
    redBGR_g = cv2.GaussianBlur(redBGR, (5, 5), cv2.BORDER_DEFAULT)

    # edge detection (canny)
    blueBGR_c = cv2.Canny(blueBGR_g, 30, 40)
    greenBGR_c = cv2.Canny(greenBGR_g, 30, 40)
    redBGR_c = cv2.Canny(redBGR_g, 30, 40)

    # dilation
    blueBGR_d = cv2.dilate(blueBGR_c, np.ones((5, 5), np.uint8), iterations = 1)
    greenBGR_d = cv2.dilate(greenBGR_c, np.ones((5, 5), np.uint8), iterations = 1)
    redBGR_d = cv2.dilate(redBGR_c, np.ones((5, 5), np.uint8), iterations = 1)

    # combine edge detected channels
    img2 = cv2.merge((blueBGR_d, greenBGR_d, redBGR_d))

    # mask combined with the blue channel
    masked = cv2.bitwise_and(img2, img2, mask = blueBGR_c)

    # dilation again
    dilated = cv2.dilate(masked, np.ones((5, 5), np.uint8), iterations = 1)

    # Hough transform - grayscale before transformation
    gray = cv2.cvtColor(dilated, cv2.COLOR_BGR2GRAY)

    # Hough transform
    for i in range(threshold, 0, -20):
        lines = cv2.HoughLines(gray, 1, np.pi/180, i)
        if lines is not None:
            if is_first == 1:
                threshold = i + 100 # threshold set from first image
                is_first = 0
            break

    for i in range(0, len(lines)):
        rho = lines[i][0][0]
        theta = lines[i][0][1]
        a = np.cos(theta)
        b = np.sin(theta)
        x0 = a * rho
        y0 = b * rho
        pt1 = (int(x0 + 1000 * (-b)), int(y0 + 1000 * (a)))
        pt2 = (int(x0 - 1000 * (-b)), int(y0 - 1000 * (a)))
        cv2.line(img, pt1, pt2, (255, 0, 0), 3, cv2.LINE_AA)

    ang = (sum(lines)/len(lines))[0][1]*180/np.pi
    cv2.imshow("subo", img)
    print("Angle: ", ang)

    c = cv2.waitKey(1)
    if c == 27:
        break
cv2.destroyAllWindows()
cap.release()
