import sys
import cv2 as cv
import numpy as np
from lib import FaceLib, FaceLoad, FaceOperator

if len(sys.argv)!=3:
    print('❌ Invalid parameters')
    exit()
energy = float(sys.argv[1])
model_dir = sys.argv[2]

facelib = FaceLib()
faceoperator = FaceOperator()

model = facelib.load(model_dir)
size = facelib.size
eigen_face = []


# train
print("🚌 Calculating Convariance Matrix...")
convar_matrix = mean_matrix = np.zeros((1, 1))
(convar_matrix, mean_matrix) = cv.calcCovarMatrix(
    model, mean_matrix, cv.COVAR_ROWS | cv.COVAR_NORMAL)

convar_matrix = convar_matrix / (size - 1)

print("🚌 Calculating Eigen Vector...")
eigen_value = eigen_vector = np.zeros((1, 1))
(r, eigen_value, eigen_vector) = cv.eigen(
    convar_matrix, eigen_value, eigen_vector)
tmp_matrix = np.zeros((1, facelib.face_num))
for i in range(size):
    if i == 0:
        tmp_matrix = mean_matrix
    else:
        tmp_matrix = np.vstack((tmp_matrix, mean_matrix))
model = model.astype(np.float64) - tmp_matrix
eigen_vector = (model.dot(eigen_vector.T)).T

print('🚌 Calculating eginface...')
value_sum = cv.sumElems(eigen_value)[0]
energy_level = value_sum*energy
energy_sum = 0
index = 0
# calculate the eigenface number using "energy"
for i in range(facelib.face_num):
    energy_sum += eigen_value[i, 0]
    if energy_sum >= energy_level:
        index = i
        break
# slice
eigen_vector = eigen_vector[0:index, :]
eigen_value = eigen_value[0:index, :]

# output the eigenface data
print('🚌 Saving eigenface vector...')
np.savetxt('output_vector.txt', eigen_vector, fmt='%f', delimiter=',')
np.savetxt('output_value.txt', eigen_value, fmt='%f', delimiter=',')

for i in range(10):
    if i == 0:
        eigen_face = faceoperator.getImg(
            eigen_vector[i], facelib.width, facelib.height)
        eigen_face = np.uint8(cv.normalize(
            eigen_face, None, 0, 255, cv.NORM_MINMAX))
    else:
        tmp = faceoperator.getImg(
            eigen_vector[i], facelib.width, facelib.height)
        tmp = np.uint8(cv.normalize(
            tmp, None, 0, 255, cv.NORM_MINMAX))
        eigen_face = np.hstack((eigen_face, tmp))

cv.imshow('Top10Eigenface', eigen_face)
print('🍰 Press any key to continue...')
cv.waitKey()
cv.destroyAllWindows()
