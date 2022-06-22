import cv2 as cv
import numpy as np
import os

cali_path = './calibration/'
bird_path = './birdseye/'
board_w = 12
board_h = 12
board_sz = (12, 12)
images = [image for image in os.listdir(cali_path) if(image.endswith('jpg'))]

criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)

objp = np.zeros((12*12, 3), np.float32)
objp[:, :2] = np.mgrid[0:12, 0:12].T.reshape(-1, 2)

objpoints = []  # store the object point
imgpoints = []  # store the object point

for image in images:
    img = cv.imread(cali_path+image)
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
    # find chess corner
    ret, corners = cv.findChessboardCorners(gray, (12, 12), None)

    if ret == True:
        objpoints.append(objp)
        corners2 = cv.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
        imgpoints.append(corners2)
        # draw the chessboard
        # cv.drawChessboardCorners(img, (12, 12), corners2, ret)

# calibration
ret, mtx, dist, rvecs, tvecs = cv.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)
file = open('params.txt', 'w')
print('🖊️ Writing...')
file.write('image_width: %d\nimage_height: %d\ncamera_matrix:\n' % gray.shape[::-1])
file.write(np.str_(mtx))
file.write('\ndistortion_coefficients:\n')
file.write(np.str_(dist))
file.close()

# bird's eye
# params we need
intrinsic = mtx
distortion = dist

bird_images = [image for image in os.listdir(bird_path) if image.endswith('jpg')]
for image in bird_images:
    # undistortion
    img = cv.imread(bird_path+image)
    dst = cv.undistort(img, intrinsic, distortion, None, intrinsic)
    gray = cv.cvtColor(dst, cv.COLOR_BGR2GRAY)

    # get the checkerboard
    ret, corners = cv.findChessboardCorners(dst, board_sz, None)

    # get subpixel accuarcy
    corners = cv.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)

    # get the image and object points
    bird_h = 600
    bird_w = 600

    objPts = np.float32([[0, 0], [bird_w, 0], [bird_w, bird_h], [0, bird_h]])

    imgPts = np.array(corners[0], np.float32)
    imgPts = np.append(imgPts, corners[board_w-1], axis=0)
    imgPts = np.append(imgPts, corners[(board_h-1)*board_w+board_w-1], axis=0)
    imgPts = np.append(imgPts, corners[(board_h-1)*board_w], axis=0)

    # test
    cv.circle(dst, (int(imgPts[0][0]), int(imgPts[0][1])), 9, (255, 0, 0), 3)
    cv.circle(dst, (int(imgPts[1][0]), int(imgPts[1][1])), 9, (0, 255, 0), 3)
    cv.circle(dst, (int(imgPts[2][0]), int(imgPts[2][1])), 9, (0, 0, 255), 3)
    cv.circle(dst, (int(imgPts[3][0]), int(imgPts[3][1])), 9, (0, 255, 255), 3)
    cv.drawChessboardCorners(dst, (12, 12), corners, ret)
    cv.imshow('corner', dst)
    cv.waitKey()

    # find the homography
    H = cv.getPerspectiveTransform(imgPts, objPts)

    # add listener to key
    Z = 0
    while True:
        H[2, 2] = Z
        # remap the image
        bird_image = cv.warpPerspective(dst, H, (bird_w, bird_h))
        cv.imshow(image, bird_image)
        key = cv.waitKey()
        if key == 117:  # u
            Z += 0.5
        if key == 100:  # d
            Z -= 0.5
        if key == 27:
            break
    cv.destroyAllWindows()
