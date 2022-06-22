import sys
import cv2 as cv
import numpy as np
from lib import FaceLib, FaceLoad, FaceOperator


person_num = 40
img_num = 10
test_num = 8
eigen_vector = np.loadtxt('output_vector.txt', delimiter=',')
eigen_value = np.loadtxt('output_value.txt', delimiter=',')
facelib = FaceLib()
faceload = FaceLoad()
faceoperator = FaceOperator()

if len(sys.argv) == 2:
    model_path = sys.argv[1]
    model = facelib.load(model_path)
    distance = eigen_vector.dot(model)
    for i in range(person_num):
        print('图', str(i+1))
        for j in range(img_num):
            matrix = faceload.load_pic(
                model_path+'/'+str(i+1)+'/'+str(j+1).rjust(2, '0'), 'png')
            face_vect = eigen_vector.dot(matrix)  # 200*1
            min_distance = cv.norm(face_vect, distance[:, 0], cv.NORM_L2)
            (row, col) = distance.shape
            min_index = 0

            for k in range(col):
                tmp_distance = cv.norm(face_vect, distance[:, k], cv.NORM_L2)
                if(tmp_distance < min_distance):
                    min_distance = tmp_distance
                    min_index = k
            print(str(min_index//test_num+1)+'/'+str(min_index % test_num+1), end=' ')
        print()

elif len(sys.argv) == 3:
    test_path = sys.argv[1]
    model_path = sys.argv[2]
    model = facelib.load(model_path)
    distance = eigen_vector.dot(model)

    matrix = faceload.load_pic(test_path, 'png')
    face_vect = eigen_vector.dot(matrix)
    min_distance = cv.norm(face_vect, distance[:, 0], cv.NORM_L2)
    (row, col) = distance.shape
    min_index = 0
    for i in range(col):
        tmp_distance = cv.norm(face_vect, distance[:, i], cv.NORM_L2)
        if(tmp_distance < min_distance):
            min_distance = tmp_distance
            min_index = i
    print('✅ Matched pic:', str(min_index//test_num+1)+'/'+str(min_index % test_num+1))
    matched_pic = faceload.load_pic(model_path+'/'+str(min_index//test_num+1)+'/'+str(min_index % test_num+1).rjust(2,'0'), 'png')
    img = np.hstack((faceoperator.getImg(matrix, facelib.width, facelib.height), faceoperator.getImg(matched_pic, facelib.width, facelib.height)))
    cv.imshow('MatchedPicture', img)
    print('🍰 Press any key to continue...')
    cv.waitKey()
    cv.destroyAllWindows()
else:
    print('❌ Invalid parameters')
