import cv2 as cv

img = cv.imread("doomTiles/STONE2.png",0)

file = open("texture.bmp", "w")

for y in range(128):
    for x in range(128):
        file.write(str(img[x,y]) + ",")
print(img[0,0])