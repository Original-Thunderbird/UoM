import numpy as np
import cv2
import sys
# import imutils #for extracting RGB ftatures
# from imutils.object_detection import non_max_suppression

cap = cv2.VideoCapture(sys.argv[1])
fgbg = cv2.createBackgroundSubtractorMOG2()
i = 0

# player detection
# the 1st going through is for collecting detections for clustering
while(1):
    ret, frame = cap.read()
    if(ret == False):
        break

    # dst = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    dst = cv2.GaussianBlur(frame, (3, 3), 0)
    # dst = cv2.medianBlur(frame, 7)

    fgmask = fgbg.apply(dst) 
    fgmask = cv2.erode(fgmask, None, iterations=2)
    # kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
    # fgmask = cv2.morphologyEx(fgmask, cv2.MORPH_OPEN, kernel)
    ret, fgmask = cv2.threshold(fgmask, 150, 255, cv2.THRESH_BINARY)
    fgmask = cv2.dilate(fgmask, None, iterations=1)

    contours, hierarchy = cv2.findContours(fgmask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
    # print(contours)
    # cv2.drawContours(frame, contours, -1, (0,255,0), 3)
    box_list = []
    if len(contours) > 0:
        for cnt in contours:
            x,y,w,h = cv2.boundingRect(cnt)
            # filter out abnormal detections, param to be adjusted
            if w*h > 2500 and w>22: # heuristic
                cropimg = frame[y:y+h, x:x+w]
                box_list.append((x,y,w,h))
                cv2.imwrite('raw/'+ str(i) + '.jpg', cropimg)
                i += 1
        # draw appropriate box, only useful in demo
        if len(box_list) > 0:
            for box in box_list:
                # cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
                cv2.rectangle(frame, (box[0], box[1]), (box[0]+box[2], box[1]+box[3]), (0, 255, 0), 2)
            

    frame = cv2.resize(frame, (720, 480))
    fgmask = cv2.resize(fgmask, (720, 480))

    cv2.imshow('fgmask', frame)
    cv2.imshow('frame', fgmask)

    k = cv2.waitKey(40) & 0xff
    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()

# player classification
