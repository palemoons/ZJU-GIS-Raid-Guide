import sys
import cv2 as cv
import numpy as np
import math


class FaceLoad:
    def __init__(self):
        self.ori_pic = []
        self.trans_pic = []
        self.equal_pic = []
        self.matrixT = []
        self.size = 0
        self.x1 = 0
        self.x2 = 0
        self.y1 = 0
        self.y2 = 0

    def load_eye_pos(self, path):
        file = open(path+'.txt', mode='r')
        txt = file.read()
        file.close()
        arr = txt.split(' ')[0:4]
        self.x1 = int(arr[0])
        self.y1 = int(arr[1])
        self.x2 = int(arr[2])
        self.y2 = int(arr[3])

    def load_pic(self, path, ext):
        self.load_eye_pos(path)
        self.ori_pic = cv.imread(path+'.'+ext, cv.COLOR_BGR2GRAY)
        (height, width) = self.ori_pic.shape[:2]

        # get center
        center = ((self.x1+self.x2)//2, (self.y1+self.y2)//2)

        # 两只眼睛倾斜的角度
        angle = math.atan((self.y2-self.y1)//(self.x2-self.x1) * 180 / np.pi)
        trans_matrix = cv.getRotationMatrix2D(center, angle, 1.0)
        # 仿射变换
        self.trans_pic = cv.warpAffine(
            self.ori_pic, trans_matrix, (width, height))
        # 中心缩放至90%
        self.trans_pic = self.trans_pic[int(
            height*0.05):int(height*0.95), int(width*0.05):int(width*0.95)]
        # 直方图均衡化
        self.equal_pic = cv.equalizeHist(self.trans_pic)
        # 转换成n*1矩阵
        self.matrixT = self.equal_pic.reshape(
            self.equal_pic.shape[0]*self.equal_pic.shape[1], 1)
        self.size = self.equal_pic.shape[0]*self.equal_pic.shape[1]

        return self.matrixT


class FaceLib:
    def __init__(self):
        self.face_num = 320
        self.person_num = 40
        self.train_num = 8
        self.width = 0
        self.height = 0
        self.size = 0
        self.lib = []

    def load(self, path):
        # path: dir path of model
        for i in range(self.person_num):
            for j in range(self.train_num):
                full_path = path+'/'+str(i+1)+'/'+str(j+1).rjust(2, '0')
                faceload = FaceLoad()
                faceload.load_pic(full_path, 'png')
                if self.size == 0:
                    self.size = faceload.size
                    self.width = faceload.equal_pic.shape[1]
                    self.height = faceload.equal_pic.shape[0]
                if (i == 0 and j == 0):
                    self.lib = faceload.matrixT
                else:
                    self.lib = np.hstack((self.lib, faceload.matrixT))

        return self.lib


class FaceOperator:
    def __init__(self):
        void = []

    def getImg(self, matrix, width, height):
        img = matrix.reshape(height, width)
        return img
