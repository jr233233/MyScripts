import matplotlib.image as mpimg
import numpy as np

img_eg = mpimg.imread("./test.jpg")
rows, cols, chs = img_eg.shape
print(f'(rows, cols) = ({rows}, {cols})')

img_eg = img_eg[:, :, :3]
img_temp = img_eg.reshape(rows, cols * 3)
U, Sigma, VT = np.linalg.svd(img_temp)

# 取前60个奇异值
sval_nums = 60
img_restruct1 = (U[:, 0:sval_nums]).dot(
    np.diag(Sigma[0:sval_nums])).dot(VT[0:sval_nums, :])
img_restruct1 = img_restruct1.reshape(rows, cols, 3).astype(np.uint8)
mpimg.imsave('./test1.jpg', img_restruct1)

# 取前120个奇异值
sval_nums = 600
img_restruct2 = (U[:, 0:sval_nums]).dot(
    np.diag(Sigma[0:sval_nums])).dot(VT[0:sval_nums, :])
img_restruct2 = img_restruct2.reshape(rows, cols, 3).astype(np.uint8)
mpimg.imsave('./test2.jpg', img_restruct2)


