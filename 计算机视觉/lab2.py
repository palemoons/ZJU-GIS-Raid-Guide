import numpy as np
import cv2 as cv

# 读图
img = cv.imread("img.png")
# 转为灰度图
image = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
# 转换为二值图像
ret, threshold = cv.threshold(image, 127, 255, cv.THRESH_BINARY)

# 轮廓查找
contours, hierarchy = cv.findContours(threshold, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
# 椭圆拟合
for contour in contours:
    if(len(contour) >= 5):
        ellipse = cv.fitEllipse(contour)
        image = cv.ellipse(img, ellipse, (0, 0, 128), 2)
    else:
        cv.drawContours(img, contour, -1, (0, 255, 0), 2)

cv.imshow('椭圆拟合', img)
print('press any key to exit...')
cv.waitKey()
cv.destroyAllWindows()
