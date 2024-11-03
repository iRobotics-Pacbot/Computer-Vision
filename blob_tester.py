import numpy as np
import cv2 as cv
import math
import pickle
import copy

import time

from cv_arena import wall_correction

# Exposure adjustment
def gamma_trans(img, gamma):
    gamma_table=[np.power(x/255.0,gamma)*255.0 for x in range(256)]
    gamma_table=np.round(np.array(gamma_table)).astype(np.uint8)
    return cv.LUT(img,gamma_table)

def calc_circularity(area, perimeter):
    circularity = 0
    if perimeter != 0:
        circularity = 4*math.pi*area / perimeter**2
    return circularity

def calc_aspect_ratio(width, height):
    aspect_ratio = 100
    if height != 0 and width != 0:
        aspect_ratio = width / height
    return aspect_ratio

def point_perspective_trans(matrix, point):
    px = (matrix[0][0]*point[0] + matrix[0][1]*point[1] + matrix[0][2]) / ((matrix[2][0]*point[0] + matrix[2][1]*point[1] + matrix[2][2]))
    py = (matrix[1][0]*point[0] + matrix[1][1]*point[1] + matrix[1][2]) / ((matrix[2][0]*point[0] + matrix[2][1]*point[1] + matrix[2][2]))
    print(px, py)
    return (int(px), int(py))
    
print("setting up")
# Open camera at default camera port
cam_width, cam_height = 1280, 800

cap = cv.VideoCapture(0, cv.CAP_DSHOW)
if not cap.isOpened():
    print("Cannot open camera")
    exit()

mtx, dist = pickle.load(open('calibration.pkl', 'rb'))

newcameramtx, roi = cv.getOptimalNewCameraMatrix(mtx, dist, (cam_width, cam_height), 1, (cam_width, cam_height))

cap.set(3, cam_width)
cap.set(4, cam_height)
cap.set(cv.CAP_PROP_AUTO_EXPOSURE,0)
cap.set(cv.CAP_PROP_EXPOSURE,-11)
cap.set(cv.CAP_PROP_AUTOFOCUS, 0)
cap.set(cv.CAP_PROP_SETTINGS, 1)
cap.set(cv.CAP_PROP_FPS, 120)
cap.set(cv.CAP_PROP_FOURCC, cv.VideoWriter.fourcc('M','J','P','G'))

print("setup successful")

num_rows = 31
num_cols = 28
row_to_pxl = cam_height/num_rows
col_to_pxl = cam_width/num_cols

param = cv.SimpleBlobDetector.Params()
param.filterByCircularity = True
param.filterByArea = True
param.minArea = 150
param.minCircularity = 0.8
param.minThreshold = 91
param.maxThreshold = 255

bot_detector = cv.SimpleBlobDetector.create(param)

ref_param = cv.SimpleBlobDetector.Params()
ref_param.filterByCircularity = False
ref_param.filterByArea = True
ref_param.minThreshold = 91
ref_param.maxThreshold = 255
ref_param.minCircularity = 0.01
ref_param.maxCircularity = 0.8
ref_param.maxArea = 110
ref_param.minArea = 30

ref_detector = cv.SimpleBlobDetector.create(ref_param)

while True:
    ret, frame = cap.read()
    curr_time = time.time()
    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        raise RuntimeError()

    frame = cv.undistort(frame, mtx, dist, None, newcameramtx)
    x, y, w, h = roi
    frame = frame[y:y+h, x:x+w]
    # Our operations on the frame come here
    # Grayscale transformation
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

    # Reduce exposure
    exposed = gamma_trans(gray,10)

    # Do Gaussian Blur to reduce noise
    blur = cv.GaussianBlur(exposed,(5,5),0)

    # Do a binary threshhold to filter out not reflective / not retroreflective objects
    ret2, thresh_img = cv.threshold(blur,91,255,cv.THRESH_BINARY)

    # Find all existing contours in the frame. Only retrieving external contours as internal ones won't be very useful in our situation where the contours we want would all be fully filled in
    
    contours =  cv.findContours(thresh_img,cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)[-2]

    # Find all existing contours in the frame. Only retrieving external contours as internal ones won't be very useful in our situation where the contours we want would all be fully filled in
    #bot = bot_detector.detect(exposed)
    #refs = ref_detector.detect(exposed)

    for c in contours:
        area = cv.contourArea(c)
        peri = cv.arcLength(c, True)
        rect = cv.minAreaRect(c)
        print("thing detected")

    print(time.time() - curr_time)

    #cv.drawContours(exposed,contours,-1,(0,255,0), 3)

    cv.imshow('frame',exposed)

    #print(refs)

    # if(len(refs) > 0):
    #     print(refs[0].pt)

    if cv.waitKey(1) == ord('q'):
        cv.imwrite("sample3.png",exposed)
        break

cap.release()
cv.destroyAllWindows()