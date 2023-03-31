import numpy as np
import cv2
from operator import itemgetter
from pickle import dump, HIGHEST_PROTOCOL
import sys
# import imutils #for extracting RGB ftatures
# from imutils.object_detection import non_max_suppression

word_bag_size = 75

def knn(trainData, testData, labels, k):
    # 计算训练样本的行数
    rowSize = trainData.shape[0]
    # 计算训练样本和测试样本的差值
    diff = np.tile(testData, (rowSize, 1)) - trainData
    # 计算差值的平方和
    sqrDiff = diff ** 2
    sqrDiffSum = sqrDiff.sum(axis=1)
    # 计算距离
    distances = sqrDiffSum ** 0.5
    # 对所得的距离从低到高进行排序,sortDistance saves indices of trainData
    sortDistance = distances.argsort()
    count = {}
    for i in range(k):
        vote = labels[sortDistance[i]]
        # cout.get(vote, 0): if vote is not found in count, then return 0
        count[vote] = count.get(vote, 0) + 1
    # 对类别出现的频数从高到低进行排序
    # itemgetter is from operator
    sortCount = sorted(count.items(), key=itemgetter(1), reverse=True)
    # 返回出现频数最高的类别
    return sortCount[0][0]

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


# load pretrained bovw model for player classification
bovw_model = np.load("MSER_model.npz", allow_pickle=True)
hist_vec = bovw_model['hist_vec']
class_vec = bovw_model['class_vec']
centers = bovw_model['centers']
label_map_from_str_to_num = list(bovw_model['label_map_from_str_to_num'])
word_label = list(range(len(centers)))

cap = cv2.VideoCapture(sys.argv[1])
size = (int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)), int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
fgbg = cv2.createBackgroundSubtractorMOG2()
hog = cv2.HOGDescriptor()
hog.setSVMDetector( cv2.HOGDescriptor_getDefaultPeopleDetector() )
# the 1st going through is for generating tracklets
# amount of maximum prediction(and then cutoff)
pred_th = 15
tracklet_pool = []
tracklet_stock = []
kalman_pool = []
t = 0
while(1):
    ret, frame = cap.read()
    if ret == False:
        break
    cv2.resize(frame, (640, 480))
    # player detection
    (rects, weights) = hog.detectMultiScale(frame, winStride=(4, 4),padding=(0, 0), scale=1.3)
    # draw the original bounding boxes
    cur_detection = []
    # delete duplicate boxes
    delete_buffer = []
    for i in range(len(rects)-1):
        for j in range(i+1,len(rects)):
            # print(str(i)+' '+str(j))
            if almost_contain(rects[i], rects[j]):
                if contain(rects[i], rects[j]):
                    delete_buffer.append(i)
                else:
                    delete_buffer.append(j)
            elif almost_contain(rects[j], rects[i]):
                if contain(rects[j], rects[i]):
                    delete_buffer.append(j)
                else:
                    delete_buffer.append(i)
    delete_buffer = list(dict.fromkeys(delete_buffer))
    # print('part:'+str(delete_buffer))
    if len(delete_buffer) > 0:
        rects = np.delete(rects, delete_buffer, axis=0)
    for (x, y, w, h) in rects:
        # filter out some false detections
        if w*h<25000 and y > 450:
            # print('x = ' + str(x) + ', y =' + str(y))
            # draw bounding box
            # cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
            
            # player classification
            cropimg = frame[y:y+h, x:x+w]
            sift = cv2.xfeatures2d_SIFT.create()
            # kp: location; des: feature vectors
            # kp, des = sift.detectAndCompute(cropimg, None)
            mser = cv2.MSER_create()
            kp1 = mser.detect(cropimg)
            kp2, des = sift.compute(cropimg, kp1)
            if des is not None:
                # build bovw model of detected subimage
                hist_of_detect = [0]*word_bag_size
                for vec in des:
                    # find the nearest center is 1-NN on centers
                    word = knn(centers, vec, word_label, 1)
                    hist_of_detect[word] += 1
                pic_label = knn(hist_vec, hist_of_detect, class_vec, 11)
                base = [x + w/2, y + h/2]
                # the last element in tuple 
                if pic_label == 0:
                    # blue
                    color = (255, 0, 0)
                elif pic_label == 1:
                    # green
                    color = (0, 255, 0)
                elif pic_label == 2:
                    # red
                    color = (0, 0, 255)
                else:
                    # ocean
                    color = (255, 255, 0)
                cv2.rectangle(frame, (x, y), (x+w, y+h), color, 2)
                cur_detection.append([x, y, w, h, base, pic_label, 0])
            
    print('t= '+str(t))
    # # detection check
    # print('detection in current frame is:')
    # for detection in cur_detection:
    #     print(detection)
    # cv2.imshow('img', frame)
    # jian = cv2.waitKey() & 0xff
    # if jian == 27:
    #     break
    
    # tracking
    tracklet_assigned = [False]*len(tracklet_pool)
    detection_assigned = [False]*len(cur_detection)
    # distance matrix
    dist_mat = np.array([[0 for col in range(len(cur_detection))] for row in range(len(tracklet_pool))], dtype='float32')
    for i in range(len(tracklet_pool)):
        # print('i = '+ str(i))
        # print(tracklet_pool[i][0])
        if len(tracklet_pool[i][1])>1:
            # initialize corresponding filter
            if len(tracklet_pool[i][1]) == 2:
                # H
                kalman_pool[i].measurementMatrix = 1. *np.eye(2)
                # Q (smaller the Q, more believable the predicted value)
                kalman_pool[i].processNoiseCov = 1e-1 *np.eye(2)
                # R
                kalman_pool[i].measurementNoiseCov = 1e-5 * np.eye(2)
                # P(k)=(I-K(k)*H)*P'(k)
                kalman_pool[i].errorCovPost = 1. *np.eye(2)
                # x(k)=x'(k)+K(k)*(z(k)-H*x'(k))
                kalman_pool[i].statePost = np.array([[tracklet_pool[i][1][-1][4][0]], [tracklet_pool[i][1][-1][4][1]]], np.float64)
                # print('detection '+ str(kalman_pool[i].statePost) + ' is fed in corresponding tracklet')
            # transition matrix A:[[1, (x_k-1 - x_k-2)/y_k-1],[(y_k-1 - y_k-2)/x_k-1]]
            # print('for tracklet '+str(i)+',last detection is= '+str(kalman_pool[i].statePost))
            kalman_pool[i].transitionMatrix = np.array([[1, (tracklet_pool[i][1][-1][4][0]-tracklet_pool[i][1][-2][4][0])/tracklet_pool[i][1][-1][4][1]],[(tracklet_pool[i][1][-1][4][1]-tracklet_pool[i][1][-2][4][1])/tracklet_pool[i][1][-1][4][0], 1]], np.float64)
            # produce prediction for current frame
            prediction = kalman_pool[i].predict()
            # check if using the right filter
            # print('historical info is:')
            # for j in range(len(tracklet_pool[i][1])):
            #     print(tracklet_pool[i][1][j])
            # print('final detection for last frame is:'+ str(kalman_pool[i].statePost))
            # print('prediction for current frame is: '+str(kalman_pool[i].statePre) + 'variable prediction is: '+ str(prediction) +'\n')
            # cv2.imshow('img', frame)
            # jian = cv2.waitKey() & 0xff
            # if jian == 27:
            #     break

        for j in range(len(cur_detection)):
            if len(tracklet_pool[i][1])>1:
                # use detection - prediction
                dist_mat[i][j] = np.linalg.norm(np.array(cur_detection[j][4])-np.array([prediction[0][0],prediction[1][0]]))
                # print('prediction is used')
            else:
                # use detection - last detection
                dist_mat[i][j] = np.linalg.norm(np.array(cur_detection[j][4])-np.array(tracklet_pool[i][1][-1][4]))
            # print('for i= '+ str(i) + ', j = ' + str(j) + ', distance = ' + str(dist_mat[i][j]))
    # print('t = '+str(t)+' pool size = '+str(len(tracklet_pool))+' detect size = '+str(len(cur_detection)))
    # the shortest distance is found and the corresponding detection is assigned to this tracklet
    # both list should be non-empty
    if len(cur_detection) * len(tracklet_pool) > 0:
        # print('\ndistance matrix = ')
        # print(dist_mat)
        # print('\n')
        min_for_col = dist_mat.min(axis=0)
        i_seq = min_for_col.argsort()
        for i in i_seq:
            # let every detection to find nearest tracklet or let every tracklet to find nearest detection?
            arg_mindist_list = np.argmin(dist_mat, axis=0)
            arg_mindist = arg_mindist_list[i]
            # temporal coherence
            if dist_mat[arg_mindist][i] < 50:
                # print('for tracklet id= '+str(arg_mindist)+', assigned detection is: '+ str(np.array([[cur_detection[i][4][0]], [cur_detection[i][4][1]]])))
                # print('so tracklet '+str(arg_mindist)+' gets detection '+str(i))

                if len(tracklet_pool[arg_mindist][1])>1:
                    # print('in the kalman filter used for correction, prediction is: '+str(kalman_pool[arg_mindist].statePre))

                    # partially believe detection
                    # kalman_pool[arg_mindist].correct(np.array([[cur_detection[i][4][0]], [cur_detection[i][4][1]]], np.float64))
                    # totally believe detection
                    kalman_pool[arg_mindist].statePost = np.array([[cur_detection[i][4][0]], [cur_detection[i][4][1]]], np.float64)

                    # jian = cv2.waitKey() & 0xff
                    # if jian == 27:
                    #     break
                    # print('final detection after frame '+ str(t)+'is '+str(kalman_pool[arg_mindist].statePost))
                    # print('params:')
                    # print('prediction(aft) is:'+ str(kalman_pool[arg_mindist].statePre))
                    # assign corrected base to final detection
                    cur_detection[i][4][0] = kalman_pool[arg_mindist].statePost[0][0]
                    cur_detection[i][4][1] = kalman_pool[arg_mindist].statePost[1][0]
                    # update w and h according to corrected base
                    # # w = (base[0] - x)*2
                    # cur_detection[i][2] = (cur_detection[i][4][0] - cur_detection[i][0])*2
                    # # h = (base[1] - y)*2
                    # cur_detection[i][1] = (cur_detection[i][4][1] - cur_detection[i][1])*2

                tracklet_pool[arg_mindist][1].append(cur_detection[i])
                tracklet_assigned[arg_mindist] = True
                detection_assigned[i] = True
                # once a detection is assigned to a tracket, it cannot be assigned to other tracklets anymore
                for j in range(len(tracklet_pool)):
                    dist_mat[j][i] = float('inf') # an arbitary 'infinity'
                for j in range(len(cur_detection)):
                    dist_mat[arg_mindist][j] = float('inf') # an arbitary 'infinity'
                
    # If there are tracklets which have not been assigned a new detection,
    # the previous location is added as an estimated location for this frame
    for i in range(len(tracklet_pool)):
        if tracklet_assigned[i] == False:
            # linear prediction, predict location of detection in current frame
            virtual_detection = tracklet_pool[i][1][-1].copy()
            if len(tracklet_pool[i][1])>1: # prediction base on historical info
                measurement = kalman_pool[i].statePre
                kalman_pool[i].correct(measurement)
                base = list(np.array(virtual_detection[4]) + (np.array(tracklet_pool[i][1][-1][4]) - np.array(tracklet_pool[i][1][-2][4])))
                # prediction out of frame:stop further prediction
                if base[0] > size[0] or base[0] < 0 or base[1] > size[1] or base[1] < 0:
                    virtual_detection[6] = pred_th-1
            else: # too few historical frames, hardcode a prediction
                base = list(np.array(virtual_detection[4]) + np.array([0.1, 0.1]))
            virtual_detection[4] = base
            # # x = base[0] - w/2
            # virtual_detection[0] = virtual_detection[4][0] - virtual_detection[2]/2
            # # y = base[1] - h/2
            # virtual_detection[1] = virtual_detection[4][1] - virtual_detection[3]/2
            # count of virtual detections
            virtual_detection[6] = virtual_detection[6] + 1
            tracklet_pool[i][1].append(virtual_detection)
            tracklet_assigned[i] = True
    # Stale tracklets which have had no new detections for a predetermined number of frames are deactivated
    delete_buffer = []
    for i in range(len(tracklet_pool)):
        if tracklet_pool[i][1][-1][6] == pred_th:
            # save index in buffer(await to be deleted from pool)
            delete_buffer.append(i)
            # save tracklet in stock, some detections that are not players(and only appears in 1 frame) may be added into tracklets
            # and have virtual detections attached, so some heuristics are needed to prevent them from entering the final tracklet stock.
            # if len(tracklet_pool[i][1])>30:
            tracklet_stock.append(tracklet_pool[i])
            # test
            # for j in range(len(tracklet_pool[i][1])):
            #     print(tracklet_pool[i][1][j])
            # exit(0)

    if len(delete_buffer) > 0:
        delete_buffer.reverse()
        for i in range(len(delete_buffer)):
            tracklet_pool.pop(delete_buffer[i])
            kalman_pool.pop(delete_buffer[i])

    # New tracklets are created for any unassigned detections with detect time
    for i in range(len(cur_detection)):
        if detection_assigned[i] == False:
            tracklet_pool.append([t, [cur_detection[i]]])
            # Kalman filters are initialized
            kalman_pool.append(cv2.KalmanFilter(2, 2, 0))

    # cv2.imshow('fgmask', frame)
    t += 1

cap.release()
cv2.destroyAllWindows()

# save all remaining tracklets
tracklet_stock.extend(tracklet_pool)

# # test
# for tracklet in tracklet_stock:
#     print(tracklet[0])
#     for detection in tracklet[1]:
#         print(detection)
#     print('\n\n')

# np.save('tracklets_raw.npy', tracklet_stock)
# dump, HIGHEST_PROTOCOL from package pickle
with open('tracklets_raw.pickle', 'wb') as fw:
    dump(tracklet_stock, fw, protocol=HIGHEST_PROTOCOL)