import numpy as np
import cv2 as cv
import math

def gamma_trans(img, gamma):
    gamma_table=[np.power(x/255.0,gamma)*255.0 for x in range(256)]
    gamma_table=np.round(np.array(gamma_table)).astype(np.uint8)
    return cv.LUT(img,gamma_table)

cap = cv.VideoCapture(0)
if not cap.isOpened():
    print("Cannot open camera")
    exit()
cam_width  = cap.get(3)   # float `width`
cam_height = cap.get(4)  # float `height`
while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame (stream end?). Exiting ...")
        break
    # Our operations on the frame come here
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)

    exposed = gamma_trans(gray,8)

    blur = cv.GaussianBlur(exposed,(5,5),0)
    ret2, thresh_img = cv.threshold(blur,91,255,cv.THRESH_BINARY)

    contours =  cv.findContours(thresh_img,cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE)[-2]

    areas = []
    corner_contours = []
    corner_points = []
    pacbot = []

    for c in contours:
        if area > 3000 or area < 700:
            continue
        area = cv.contourArea(c)
        perimeter = cv.arcLength(c,True)
        circularity = 0
        if perimeter != 0:
            circularity = 4*math.pi*area / perimeter**2
        aspect_ratio = 100
        if abs(1-circularity) < 0.05 and area > 300:
            pacbot.append(c)
            cv.drawContours(frame, [c], -1, (0,255,0), 3)
            continue
        rect = cv.minAreaRect(c)
        (x, y), (width, height), angle = rect
        if height != 0 and width != 0:
            aspect_ratio = width / height
        if abs(aspect_ratio - 1) < 0.2:
            box = cv.boxPoints(rect)
            box = np.intp(box)
            cv.drawContours(frame, [box], -1, (0,255,0), 2)
            corner_contours.append(c)
            corner_points.append([x,y])
    
    if len(corner_contours) == 4:
        num_rows = 5
        num_cols = 8
        row_to_pxl = 480//num_rows
        col_to_pxl = 640//num_cols
        pac_pos = (0,0)
        if len(pacbot) != 0:
            pac_pos, r = cv.minEnclosingCircle(pacbot[0])
            center = (int(pac_pos[0]),int(pac_pos[1]))
            radius = int(r)
            cv.circle(frame,center,radius,(0,255,0),2)
        corner_points.sort()
        corner_pts_32 = np.float32(corner_points)
        target_pts = np.float32([[0,0],[0,480],[640,0],[640,480]])
        matrix = cv.getPerspectiveTransform(corner_pts_32,target_pts)
        result = cv.warpPerspective(frame,matrix,(640,480))
        for i in range(0,num_rows):
            cv.line(result,(0,row_to_pxl*i),(640,row_to_pxl*i),(0,255,0),2)
        for i in range(0,num_cols):
            cv.line(result,(col_to_pxl*i,0),(col_to_pxl*i,480),(0,255,0),2)

        px = (matrix[0][0]*pac_pos[0] + matrix[0][1]*pac_pos[1] + matrix[0][2]) / ((matrix[2][0]*pac_pos[0] + matrix[2][1]*pac_pos[1] + matrix[2][2]))
        py = (matrix[1][0]*pac_pos[0] + matrix[1][1]*pac_pos[1] + matrix[1][2]) / ((matrix[2][0]*pac_pos[0] + matrix[2][1]*pac_pos[1] + matrix[2][2]))
        pac_pos_after = (int(px), int(py)) # after transformation

        pac_x = math.floor(pac_pos_after[0]/col_to_pxl)
        pac_y = math.floor(pac_pos_after[1]/row_to_pxl)
        print(pac_x,pac_y)
        cv.circle(result,(col_to_pxl*pac_x,row_to_pxl*pac_y),50,(0,255,255),-1)

        cv.imshow('frame', result)
    else:
        cv.imshow('frame', frame)

    # Display the resulting frame
    
    if cv.waitKey(1) == ord('q'):
        break
# When everything done, release the capture
cap.release()
cv.destroyAllWindows()

# circle
        # (x,y), r = cv.minEnclosingCircle(c)
        # center = (int(x),int(y))
        # radius = int(r)
        # cv.circle(frame,center,radius,(0,255,0),2)

# best filter
    # best_contour = []
    # best_area = 0
    # for c in contours:
    #     area = cv.contourArea(c)
    #     if area > best_area:
    #         best_contour = []
    #         best_contour.append(c)
    # if len(best_contour) != 0:
    #     (x,y), r = cv.minEnclosingCircle(best_contour[0])
    #     center = (int(x),int(y))
    #     radius = int(r)
    #     cv.circle(frame,center,radius,(0,255,0),2)