import numpy as np
import cv2 as cv

import os
os.system("")

# Variables used in the arena grid
I = 1 # wall
o = 0  # pellet
e = 0  # empty space
O = 0  # powerpoint
n = 0  # untouchable
P = 0  # pacbot
v = 0  # visited
original_matrix = np.rot90([
    [I, I, I, I, I, I, I, I, I, I, I, I, e, e, e, I,
     I, I, e, e, e, I, I, I, I, I, I, I, I, I, I],
    [I, o, o, o, o, I, I, O, o, o, o, I, e, e, e, I,
     v, I, e, e, e, I, o, o, o, o, o, O, o, o, I],
    [I, o, I, I, o, I, I, o, I, I, o, I, e, e, e, I,
     v, I, e, e, e, I, o, I, I, o, I, I, I, o, I],
    [I, o, I, I, o, o, o, o, I, I, o, I, e, e, e, I,
     v, I, e, e, e, I, o, I, I, o, I, I, I, o, I],
    [I, o, I, I, o, I, I, I, I, I, o, I, e, e, e, I,
     v, I, e, e, e, I, o, I, I, o, I, I, I, o, I],
    [I, o, I, I, o, I, I, I, I, I, o, I, I, I, I, I,
     I, I, I, I, I, I, o, I, I, o, I, I, I, o, I],
    [I, o, I, I, o, o, o, o, o, o, o, o, o, o, o, o,
     o, o, o, o, o, o, o, o, o, o, o, o, o, o, I],
    [I, o, I, I, I, I, I, o, I, I, o, I, I, I, I, I,
     v, I, I, I, I, I, I, I, I, o, I, I, I, o, I],
    [I, o, I, I, I, I, I, o, I, I, o, I, I, I, I, I,
     v, I, I, I, I, I, I, I, I, o, I, I, I, o, I],
    [I, o, I, I, o, o, o, o, I, I, o, v, v, v, v, v,
     v, v, v, v, I, I, o, o, o, o, I, I, I, o, I],
    [I, o, I, I, o, I, I, o, I, I, o, I, I, v, I, I,
     I, I, I, v, I, I, o, I, I, o, I, I, I, o, I],
    [I, o, I, I, o, I, I, o, I, I, o, I, I, v, I, n,
     n, n, I, v, I, I, o, I, I, o, I, I, I, o, I],
    [I, o, o, o, o, I, I, o, o, o, o, I, I, v, I, n,
     n, n, I, v, v, v, o, I, I, o, o, o, o, o, I],
    [I, o, I, I, I, I, I, v, I, I, I, I, I, v, I, n,
     n, n, n, v, I, I, I, I, I, o, I, I, I, I, I],
    [I, o, I, I, I, I, I, v, I, I, I, I, I, v, I, n,
     n, n, n, v, I, I, I, I, I, o, I, I, I, I, I],
    [I, o, o, o, o, I, I, o, o, o, o, I, I, v, I, n,
     n, n, I, v, v, v, o, I, I, o, o, o, o, o, I],
    [I, o, I, I, o, I, I, o, I, I, o, I, I, v, I, n,
     n, n, I, v, I, I, o, I, I, o, I, I, I, o, I],
    [I, o, I, I, o, I, I, o, I, I, o, I, I, v, I, I,
     I, I, I, v, I, I, o, I, I, o, I, I, I, o, I],
    [I, o, I, I, o, o, o, o, I, I, o, v, v, v, v, v,
     v, v, v, v, I, I, o, o, o, o, I, I, I, o, I],
    [I, o, I, I, I, I, I, o, I, I, o, I, I, I, I, I,
     v, I, I, I, I, I, I, I, I, o, I, I, I, o, I],
    [I, o, I, I, I, I, I, o, I, I, o, I, I, I, I, I,
     v, I, I, I, I, I, I, I, I, o, I, I, I, o, I],
    [I, o, I, I, o, o, o, o, o, o, o, o, o, o, o, o,
     o, o, o, o, o, o, o, o, o, o, o, o, o, o, I],
    [I, o, I, I, o, I, I, I, I, I, o, I, I, I, I, I,
     I, I, I, I, I, I, o, I, I, o, I, I, I, o, I],
    [I, o, I, I, o, I, I, I, I, I, o, I, e, e, e, I,
     v, I, e, e, e, I, o, I, I, o, I, I, I, o, I],
    [I, o, I, I, o, o, o, o, I, I, o, I, e, e, e, I,
     v, I, e, e, e, I, o, I, I, o, I, I, I, o, I],
    [I, o, I, I, o, I, I, o, I, I, o, I, e, e, e, I,
     v, I, e, e, e, I, o, I, I, o, I, I, I, o, I],
    [I, o, o, o, o, I, I, O, o, o, o, I, e, e, e, I,
     v, I, e, e, e, I, o, o, o, o, o, O, o, o, I],
    [I, I, I, I, I, I, I, I, I, I, I, I, e, e, e, I, I, I, e, e, e, I, I, I, I, I, I, I, I, I, I]], k=2)

cv_matrix = []

physical_image = cv.imread("physical_arena.jpeg")
assert physical_image is not None, "file could not be read, check with os.path.exists()"

res = cv.resize(physical_image,None,fx=0.3, fy=0.3, interpolation = cv.INTER_CUBIC)

top_left_corner = [100,182]
bottom_left_corner = [93,435]
top_right_corner = [353,53]
bottom_right_corner = [380,545]

# cv.circle(res,top_left_corner,5,(0,255,255),2)
# cv.circle(res,bottom_left_corner,5,(0,255,255),2)
# cv.circle(res,top_right_corner,5,(0,255,255),2)
# cv.circle(res,bottom_right_corner,5,(0,255,255),2)

corner_points = [top_left_corner,bottom_left_corner, top_right_corner, bottom_right_corner]
corner_pts_32 = np.float32(corner_points)

target_pts = [[0,0],[0,480],[480,0],[480,480]]
target_pts_32 = np.float32(target_pts)

matrix = cv.getPerspectiveTransform(corner_pts_32,target_pts_32)
transformed = cv.warpPerspective(res,matrix,(480,480))

# cv.imshow('physical_image',transformed)
# cv.waitKey(0)
# cv.destroyAllWindows()

print(original_matrix)