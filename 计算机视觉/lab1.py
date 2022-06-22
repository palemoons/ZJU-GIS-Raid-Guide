import numpy as np
import cv2 as cv
import os

# define params
fps = 60
pptTime = 2
info = 'Yuenan Li 3190100181'
path = input("图片与视频路径：")

# get video name
video = [video for video in os.listdir(path) if video.endswith(".avi")][0]

# define VideoWriter
out = cv.VideoWriter('output.avi', 0, fps, (1440, 810))

# define font
font = cv.FONT_HERSHEY_SIMPLEX

# generate an cover
cover = np.zeros((810, 1440, 3), np.uint8)
cv.putText(cover, 'OpenCV Lab1', (640, 400), font, 1, (255, 0, 0), 4, cv.LINE_AA)
for i in range(fps * pptTime):
    out.write(cover)

# handle images in dir
images = []
for img in os.listdir(path):
    if img.endswith(".jpg"):
        images.append(img)

for img in images:
    frame = cv.imread(img)
    frame = cv.resize(frame, (1440, 810))
    cv.putText(frame, info, (0, 50), font, 1, (255, 0, 0), 4, cv.LINE_AA)
    for i in range(fps * pptTime):
        out.write(frame)

# handle video in dir
video = cv.VideoCapture(video)
while video.isOpened():
  ret, frame = video.read()
  if not ret:
    break
  frame = cv.resize(frame, (1440, 810))
  cv.putText(frame, info, (0, 50), font, 1, (255, 0, 0), 4, cv.LINE_AA)
  out.write(frame)

cv.destroyAllWindows()
video.release()
out.release()
print("视频output.avi生成成功")