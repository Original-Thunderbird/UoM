import cv2 
import numpy as np
import os
import operator
from SIFT_build import ROI_type
from SIFT_build import SIFT_TRUE_MSER_FALSE
from SIFT_build import word_bag_size

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
    sortCount = sorted(count.items(), key=operator.itemgetter(1), reverse=True)
    # 返回出现频数最高的类别
    return sortCount[0][0]

bovw_model = np.load(ROI_type+"_model.npz", allow_pickle=True)
hist_vec = bovw_model['hist_vec']
class_vec = bovw_model['class_vec']
centers = bovw_model['centers']
label_map_from_str_to_num = list(bovw_model['label_map_from_str_to_num'])

word_label = list(range(len(centers)))

i = 0
# row for original label, column for detected label
conf_mat_tr = [[0 for col in range(len(label_map_from_str_to_num))] for row in range(len(label_map_from_str_to_num))]
for root, _, filenames in os.walk('orig_train'):
    if len(filenames) > 0:
        for filename in filenames:
            # print(filename)
            img = cv2.imread(root+ '/'+ filename)
            sift = cv2.xfeatures2d_SIFT.create()
            # kp: location
            # kp: location
            if SIFT_TRUE_MSER_FALSE == True:
                kp, des = sift.detectAndCompute(img,None)
            else:
                mser = cv2.MSER_create()
                kp1 = mser.detect(img)
                kp2, des = sift.compute(img, kp1)
            if des is not None:
                # build bovw model of detected subimage
                hist_of_detect = [0]*word_bag_size
                for vec in des:
                    # find the nearest center is 1-NN on centers
                    word = knn(centers, vec, word_label, 1)
                    hist_of_detect[word] += 1
                pic_label = knn(hist_vec, hist_of_detect, class_vec, 11)
            # else:
            #     pic_label = -1
            # true_positive, false_positive, false_negative statistics
                orig_label = label_map_from_str_to_num.index(filename.split('_',1)[0])
                conf_mat_tr[orig_label][pic_label] += 1
            # write pic with label in filename
            # print('for pic'+filename+', label is'+str(pic_label))
            # cv2.imwrite('raw_aft/'+ filename[:-4] + '_label=' + str(pic_label) + '.jpg', img)


# row for original label, column for detected label
conf_mat_te = [[0 for col in range(len(label_map_from_str_to_num))] for row in range(len(label_map_from_str_to_num))]
for root, _, filenames in os.walk('orig_test'):
    if len(filenames) > 0:
        for filename in filenames:
            # print(filename)
            img = cv2.imread(root+ '/'+ filename)
            sift = cv2.xfeatures2d_SIFT.create()
            # kp: location
            # kp: location
            if SIFT_TRUE_MSER_FALSE == True:
                kp, des = sift.detectAndCompute(img,None)
            else:
                mser = cv2.MSER_create()
                kp1 = mser.detect(img)
                kp2, des = sift.compute(img, kp1)
            if des is not None:
                # build bovw model of detected subimage
                hist_of_detect = [0]*word_bag_size
                for vec in des:
                    # find the nearest center is 1-NN on centers
                    word = knn(centers, vec, word_label, 1)
                    hist_of_detect[word] += 1
                pic_label = knn(hist_vec, hist_of_detect, class_vec, 11)
            # else:
            #     pic_label = -1
            # true_positive, false_positive, false_negative statistics
                orig_label = label_map_from_str_to_num.index(filename.split('_',1)[0])
                conf_mat_te[orig_label][pic_label] += 1
            # write pic with label in filename
            # print('for pic'+filename+', label is'+str(pic_label))
            # cv2.imwrite('raw_aft/'+ filename[:-4] + '_label=' + str(pic_label) + '.jpg', img)

# training & testing precision, recall and accuracy
diag_tr = np.array([conf_mat_tr[i][i] for i in range(len(label_map_from_str_to_num))])
sum_tr = np.array(conf_mat_tr).sum()
precision_tr = diag_tr/np.array(conf_mat_tr).sum(axis=0)
prc_tr_av = sum(precision_tr)/len(precision_tr)
recall_tr = diag_tr/np.array(conf_mat_tr).sum(axis=1)
rcl_tr_av = sum(recall_tr)/len(recall_tr)
acc_tr = diag_tr.sum()/sum_tr

diag_te = np.array([conf_mat_te[i][i] for i in range(len(label_map_from_str_to_num))])
sum_te = np.array(conf_mat_te).sum()
precision_te = diag_te/np.array(conf_mat_te).sum(axis=0)
prc_te_av = sum(precision_te)/len(precision_te)
recall_te = diag_te/np.array(conf_mat_te).sum(axis=1)
rcl_te_av = sum(recall_te)/len(recall_te)
acc_te = diag_te.sum()/sum_te

print('label sequence:' + str(label_map_from_str_to_num))
print('training:\nprecision=' + str(precision_tr) + '\nrecall=' + str(recall_tr) + '\navg_precision=' + str(prc_tr_av) + '\navg_recall=' + str(rcl_tr_av) + '\naccuracy=' + str(acc_tr))
print(str(conf_mat_tr[0]) + '\n' + str(conf_mat_tr[1]) + '\n' + str(conf_mat_tr[2]) + '\n' + str(conf_mat_tr[3]) + '\n')
print('testing:\nprecision=' + str(precision_te) + '\nrecall=' + str(recall_te) + '\navg_precision=' + str(prc_te_av) + '\navg_recall=' + str(rcl_te_av) + '\naccuracy=' + str(acc_te))
print(str(conf_mat_te[0]) + '\n' + str(conf_mat_te[1]) + '\n' + str(conf_mat_te[2]) + '\n' + str(conf_mat_te[3]) + '\n')
