import cv2 as cv

import_path = "./doom_textures/"
export_path = "./textures/"

file_name = "GRAYPOIS"
file_type = ".png"


x_size = 64
y_size = 64


img = cv.imread(import_path + file_name + file_type, cv.IMREAD_COLOR)

file = open(export_path + file_name + "_c.bmp", "w")


for x in range(0,x_size):
    for y in range(0,y_size):
        file.write(str(img[x,y][2]) + "," + str(img[x,y][1]) + "," + str(img[x,y][0]) + ",")

