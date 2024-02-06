import numpy as np
import cv2 as cv
import math

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
    return (int(px), int(py))

# Open camera at default camera port
cap = cv.VideoCapture(0)
if not cap.isOpened():
    print("Cannot open camera")
    exit()
cam_width  = cap.get(3)  # float `width`
cam_height = cap.get(4)  # float `height`

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
    # Our operations on the frame come here
    # Grayscale transformation
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

    # Reduce exposure
    exposed = gamma_trans(gray,8)

    # Do Gaussian Blur to reduce noise
    blur = cv.GaussianBlur(exposed,(5,5),0)

    # Do a binary threshhold to filter out not reflective / not retroreflective objects
    ret2, thresh_img = cv.threshold(blur,91,255,cv.THRESH_BINARY)

    # Find all existing contours in the frame. Only retrieving external contours as internal ones won't be very useful in our situation where the contours we want would all be fully filled in
    contours =  cv.findContours(thresh_img,cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)[-2]

    areas = []
    corner_contours = []
    corner_points = []
    pacbot = []

    # looking through every contour to look for specific ones we want
    for c in contours:
        # Contours too large or too small are most likely noise or ambient light. This can probably be removed later because it was a measure mostly to deal with the environment I tested the code in. Retrorefractor should make things a lot cleaner
        area = cv.contourArea(c)
        if area > 3000 or area < 700:
            continue
        # calculating circularity to differentiate between the pacbot and the corner of the arena. The corners should be squares and the pacbot would be circular. 
        perimeter = cv.arcLength(c,True)
        circularity = calc_circularity(area, perimeter)
        # perfect circles have a circularity of 1. Identify circles by detecting objects with a circularity of at least 0.95. The filter for small area can be removed if noises are not significant.
        if abs(1-circularity) < 0.05 and area > 300:
            # add the contour to the potential pacbot contour list and draw it.
            pacbot.append(c)
            cv.drawContours(frame, [c], -1, (0,255,0), 3)
            continue
        # If the contour is not the pacbot, we try to confirm whether it is the corner by finding the bounding rectangle of the contour
        rect = cv.minAreaRect(c)
        (x, y), (width, height), angle = rect
        # Calculate aspect ratio to identify squares
        aspect_ratio = calc_aspect_ratio(width, height)
        # Squares have aspect ratio 1. Accept error range 0.2. 
        if abs(aspect_ratio - 1) < 0.2:
            # draw the box contour and add it to the list if in accepted range
            box = cv.boxPoints(rect)
            box = np.intp(box)
            cv.drawContours(frame, [box], -1, (0,255,0), 2)
            corner_contours.append(c)
            corner_points.append([x,y])
    
    # continue with the transformation and locating step only if there are 4 corners
    if len(corner_contours) == 4:
        # not correct number of rows and columns, need to be adjusted later
        num_rows = 5
        num_cols = 8
        row_to_pxl = 480//num_rows
        col_to_pxl = 640//num_cols
        pac_pos = (0,0)

        # If pacbot contour is detected, draws a circle at its location
        if len(pacbot) != 0:
            pac_pos, r = cv.minEnclosingCircle(pacbot[0])
            center = (int(pac_pos[0]),int(pac_pos[1]))
            radius = int(r)
            cv.circle(frame,center,radius,(0,255,0),2)
        
        # sort to correspond with the transformation matrix
        corner_points.sort()
        corner_pts_32 = np.float32(corner_points)
        target_pts = np.float32([[0,0],[0,480],[640,0],[640,480]])
        # use a perspective transformation matrix to make the detected arena fit the screen, may not be needed since the camera position will be fixed. 
        matrix = cv.getPerspectiveTransform(corner_pts_32,target_pts)
        result = cv.warpPerspective(frame,matrix,(640,480))

        # draws the arena with lines
        for i in range(0,num_rows):
            cv.line(result,(0,row_to_pxl*i),(640,row_to_pxl*i),(0,255,0),2)
        for i in range(0,num_cols):
            cv.line(result,(col_to_pxl*i,0),(col_to_pxl*i,480),(0,255,0),2)

        # transform the pacbot position into a new position that corresponds with the frame after the perspective transformation
        pac_pos_after = point_perspective_trans(matrix, pac_pos)

        # find approximate node coordinates
        pac_x = math.floor(pac_pos_after[0]/col_to_pxl)
        pac_y = math.floor(pac_pos_after[1]/row_to_pxl)
        print(pac_x,pac_y)

        # draw a circle at the approximated coordinates
        cv.circle(result,(col_to_pxl*pac_x,row_to_pxl*pac_y),50,(0,255,255),-1)

        # shows the result
        cv.imshow('frame', result)
    else:
        cv.imshow('frame', frame)
    
    # quit detection with 'q'
    if cv.waitKey(1) == ord('q'):
        break
# When everything done, release the capture
cap.release()
cv.destroyAllWindows()