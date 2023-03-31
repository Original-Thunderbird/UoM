import cv2
import numpy as np
import sys

cap = cv2.VideoCapture(sys.argv[1])
fgbg = cv2.createBackgroundSubtractorMOG2()
hog = cv2.HOGDescriptor()
hog.setSVMDetector( cv2.HOGDescriptor_getDefaultPeopleDetector() )

four_cc = cv2.VideoWriter_fourcc(*"mp4v")
fps =int(cap.get(cv2.CAP_PROP_FPS))
size = (int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)), int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
videoWriter = cv2.VideoWriter(sys.argv[2], four_cc, fps, size)

def contain(r1, r2):
    return r1[0] < r2[0] < r2[0]+r2[2] < r1[0]+r1[2] and r1[1] < r2[1] < r2[1]+r2[3] < r1[1]+r1[3]

def almost_contain(r1, r2):
    # print('r1 = '+str(r1)+', r2 = '+str(r2))
    x1,x2 = r1[0], r1[0]+r1[2]
    y1,y2 = r1[1], r1[1]+r1[3]

    x3,x4 = r2[0], r2[0]+r2[2]
    y3,y4 = r2[1], r2[1]+r2[3]

    if not ((x2<=x3 or x4<=x1) and (y2 <= y3 or y4<=y1)):
        lens = min(x2, x4) - max(x1, x3)
        wide = min(y2, y4) - max(y1, y3)
        # duplicate area
        area = lens*wide
        # print('area: '+str(area))
        if area/(r2[2]*r2[3]) > 0.85:
            return True
    return False

t = 0
while(1):
    print(t)
    ret, frame = cap.read()
    if ret == False:
        break
    cv2.resize(frame, (640, 480))
    part = frame.copy()
    # comp = frame.copy()
    # player detection
    (rects, weights) = hog.detectMultiScale(frame, winStride=(4, 4),padding=(0, 0), scale=1.3)
    # print('before deletion, rects = '+str(rects))
    # rects1 = rects.copy()
    rects2 = rects.copy()
    # for (x, y, w, h) in rects:
    #         # filter out some false detections
    #         if w*h<25000 and y > 450:
    #             # draw bounding box
    #             cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 0, 255), 2)

    # # print('before deletion, rects1 = '+str(rects1))
    # # delete duplicate boxes
    # delete_buffer = []
    # for i in range(len(rects1)-1):
    #     for j in range(i+1,len(rects1)):
    #         if contain(rects1[i], rects1[j]):
    #             delete_buffer.append(i)
    #         elif contain(rects1[j], rects1[i]):
    #             delete_buffer.append(j)
    # delete_buffer = list(dict.fromkeys(delete_buffer))
    # # print('comp:'+str(delete_buffer))
    # if len(delete_buffer) > 0:
    #     rects1 = np.delete(rects1, delete_buffer, axis=0)
    # # print('after deletion, rects1 = '+str(rects1))
    # for (x, y, w, h) in rects1:
    #     # filter out some false detections
    #     if w*h<25000 and y > 450:
    #         # draw bounding box
    #         cv2.rectangle(comp, (x, y), (x+w, y+h), (0, 0, 255), 2)
    
    # print('before deletion, rects2 = '+str(rects2))
    # delete duplicate boxes
    delete_buffer = []
    for i in range(len(rects2)-1):
        for j in range(i+1,len(rects2)):
            # print(str(i)+' '+str(j))
            if almost_contain(rects2[i], rects2[j]):
                if contain(rects2[i], rects2[j]):
                    delete_buffer.append(i)
                else:
                    delete_buffer.append(j)
            elif almost_contain(rects2[j], rects2[i]):
                if contain(rects2[j], rects2[i]):
                    delete_buffer.append(j)
                else:
                    delete_buffer.append(i)
    delete_buffer = list(dict.fromkeys(delete_buffer))
    # print('part:'+str(delete_buffer))
    if len(delete_buffer) > 0:
        rects2 = np.delete(rects2, delete_buffer, axis=0)
    # print('after deletion, rects2 = '+str(rects2))
    for (x, y, w, h) in rects2:
        # filter out some false detections
        if w*h<25000 and y > 450:
            # draw bounding box
            cv2.rectangle(part, (x, y), (x+w, y+h), (0, 0, 255), 2)
    
    t += 1
    videoWriter.write(part)
    # cv2.imshow('orig', frame)
    # cv2.imshow('part', part)
    # cv2.imshow('comp', comp)
    # k = cv2.waitKey() & 0xff
    # if k == 27:
    #     break

cap.release()
cv2.destroyAllWindows()