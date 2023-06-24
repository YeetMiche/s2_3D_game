import cv2 as cv

file_name = "./doom_textures/SKSPINE1"
file_type = ".png"
x_size = 128
y_size = 128


img = cv.imread(file_name + file_type, cv.IMREAD_COLOR)

file = open(file_name + "_c.bmp", "w")

for x in range(x_size):
    for y in range(y_size):
        file.write(str(img[x,y][2]) + "," + str(img[x,y][1]) + "," + str(img[x,y][0]) + ",")
        if img[x,y][0] > 255:
            print(img[x,y])
