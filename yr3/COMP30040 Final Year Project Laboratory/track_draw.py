import cv2
from pickle import load
import sys

stepwise = True
# tracklet_stock = list(np.load('tracklets_clear.npy', allow_pickle=True))
if stepwise == True:
    with open('tracklets_sin.pickle', 'rb') as fr:
        tracklet_stock = load(fr)
else:
    with open('tracklets_clear.pickle', 'rb') as fr:
        tracklet_stock = load(fr)
cap = cv2.VideoCapture(sys.argv[1])
# parameter preparation
# orig_four_cc = int(cap.get(cv2.CAP_PROP_FOURCC))
# b = []
# b.append(chr(orig_four_cc & 255))
# b.append(chr((orig_four_cc >> 8) & 255))
# b.append(chr((orig_four_cc >> 16) & 255))
# b.append(chr((orig_four_cc >> 24) & 255))
# four_cc = cv2.VideoWriter_fourcc(b[0],b[1],b[2],b[3]) # case sensitive!
four_cc = cv2.VideoWriter_fourcc(*"mp4v")
fps =int(cap.get(cv2.CAP_PROP_FPS))
size = (int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)), int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))

tracklet_buffer = []
id_buffer = []
# draw labeled detections in video
videoWriter = cv2.VideoWriter(sys.argv[2], four_cc, fps, size)
t = 0
while(1):
    print('t='+str(t))
    ret, frame = cap.read()
    if ret == False:
        break
    cv2.resize(frame, (640, 480))
    # pop tracklet to render list
    while len(tracklet_stock)>0 and t == tracklet_stock[0][0]:
        # if tracklet_stock[0][1][0][5] == 0:
        tracklet_buffer.append(tracklet_stock[0][1])
        id_buffer.append(tracklet_stock[0][0])
        tracklet_stock.pop(0)
        

    # print(len(tracklet_buffer))
    # for i in range(len(tracklet_buffer)):
    #     print('len:'+str(len(tracklet_buffer[i])))
    #     for j in range(len(tracklet_buffer[i])):
    #         print(tracklet_buffer[i][j], '\n')
    #     print('\n')
    
    # render tracklets
    for i in range(len(tracklet_buffer)):
        box_info = tracklet_buffer[i][0]
        # assign color of bounding box
        if box_info[5] == 0:
            # blue
            color = (255, 0, 0)
        elif box_info[5] == 1:
            # green
            color = (0, 255, 0)
        elif box_info[5] == 2:
            # red
            color = (0, 0, 255)
        else:
            # ocean
            color = (255, 255, 0)
        # draw bounding box
        print(str(tracklet_buffer[i][0])+' id = '+str(id_buffer[i]))
        # weight center
        cv2.circle(frame, (int(box_info[4][0]), int(box_info[4][1])), 3, color, -1)
        # bounding box
        cv2.rectangle(frame, (box_info[0], box_info[1]), (box_info[0] + box_info[2], box_info[1] + box_info[3]), color, 2)
        tracklet_buffer[i].pop(0)
    if stepwise == True:
        cv2.imshow('img', frame)

    # delete empty tracklet from buffer
    delete_pool = []
    for i in range(len(tracklet_buffer)):
        if len(tracklet_buffer[i]) == 0:
            # save index in buffer(await to be deleted from pool)
            delete_pool.append(i)
    if len(delete_pool) > 0:
        # delete from aft to fore to avoid index change of previous tracklets
        delete_pool.reverse()
        for i in range(len(delete_pool)):
            tracklet_buffer.pop(delete_pool[i])
            id_buffer.pop(delete_pool[i])
        
    videoWriter.write(frame)
    if stepwise == True:
        k = cv2.waitKey() & 0xff
    else:
        k = cv2.waitKey(1) & 0xff
    if k == 27:
        break
    t += 1
cap.release()
cv2.destroyAllWindows()