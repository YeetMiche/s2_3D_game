import cv2 as cv

img = cv.imread("CLAYBRICKS.png")

file = open("CLAYBRICKS.bmp", "w")

for x in range(32):
    for y in range(32):
        file.write(str(img[x,y][0]) + "," + str(img[x,y][1]) + "," + str(img[x,y][2]) + ",")
