import numpy as np
import cv2 as cv
import math
import pickle
import copy

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

ref_param = copy.deepcopy(param)
ref_param.minCircularity = 0
ref_param.maxCircularity = 0.8
ref_param.maxArea = 110
param.minArea = 30

ref_detector = cv.SimpleBlobDetector.create(ref_param)

def capture_loc() -> list[int,int]:
    ret, frame = cap.read()
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

    # Find all existing contours in the frame. Only retrieving external contours as internal ones won't be very useful in our situation where the contours we want would all be fully filled in
    bot = bot_detector.detect(exposed)
    refs = ref_detector.detect(exposed)
    
    if len(refs) == 4:
        # not correct number of rows and columns, need to be adjusted later
        pac_pos = bot[0].pt

        # if len(bot) != 0:
        #     pac_pos, r = cv.minEnclosingCircle(pacbot[0])
        
        # sort to correspond with the transformation matrix
        refs.sort()
        corner_pts_32 = np.float32(refs)
        target_pts = np.float32([[0+col_to_pxl/2,0+row_to_pxl/2],[0+col_to_pxl/2,cam_height-row_to_pxl/2],[cam_width-col_to_pxl/2,0+row_to_pxl/2],[cam_width-col_to_pxl/2,cam_height-row_to_pxl/2]])
        # use a perspective transformation matrix to make the detected arena fit the screen, may not be needed since the camera position will be fixed. 
        matrix = cv.getPerspectiveTransform(corner_pts_32,target_pts)
        result = cv.warpPerspective(frame,matrix,(cam_width,cam_height))

        # transform the pacbot position into a new position that corresponds with the frame after the perspective transformation
        pac_pos_after = point_perspective_trans(matrix, pac_pos)

        # find approximate node coordinates
        pac_x = math.floor(pac_pos_after[0]/col_to_pxl)
        pac_y = math.floor(pac_pos_after[1]/row_to_pxl)
        print(pac_x,pac_y)
        return [pac_x,pac_y]

def clean():
    cap.release()
    cv.destroyAllWindows()

if __name__ == "__main__":
    # When everything done, release the capture
    cap.release()
    cv.destroyAllWindows()