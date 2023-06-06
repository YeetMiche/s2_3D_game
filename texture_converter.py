import cv2 as cv

img = cv.imread("test.png",0)

file = open("test.bmp", "w")

for y in range(16):
    for x in range(16):
        file.write(str(img[x,y]) + ",")
print(img[0,0])