import cv2 as cv

file_name = "test"
file_type = ".png"
x_size = 16
y_size = 16


img = cv.imread(file_name + file_type)

file = open(file_name + "_c.bmp", "w")

for x in range(x_size):
    for y in range(y_size):
        file.write(str(img[x,y][2]) + "," + str(img[x,y][1]) + "," + str(img[x,y][0]) + ",")
