import cv2 
import numpy as np
import os
import operator

def knn(trainDataSize, trainData, testData, labels, k):
    # 计算训练样本和测试样本的差值
    diff = np.tile(testData, (trainDataSize, 1)) - trainData
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

colors = ('b', 'g', 'r')
binsize = 16 # hyperparameter
sum_len = int(256/binsize)
player_label = 0
hist_vec = [] #len*128 after collection
class_vec = []
label_map_from_str_to_num = []
#collect RGB feature vectors
for root, _, filenames in os.walk('orig_train'):
    if len(filenames) > 0:
        label_map_from_str_to_num.append(root[11:])
        eg_num = 0
        for filename in filenames:
            vec = []
            # print(filename)
            img = cv2.imread(root+ '/'+ filename)
            if img is None:
                print('for' + filename + ',img is none')
            channels = cv2.split(img)
            
            for (channels, color) in zip(channels, colors):
                hist = cv2.calcHist([channels], [0], None, [256], [0, 256])
                bin_hist = []
                for i in range(binsize):
                    sigma = 0
                    for j in range(sum_len*i, sum_len*(i+1)):
                        sigma += hist[j][0]
                    bin_hist.append(sigma)
                vec.extend(bin_hist)
            hist_vec.append(vec)
            class_vec.append(player_label)
        player_label += 1


hist_vec_len = len(hist_vec)

# row for original label, column for detected label
conf_mat_te = [[0 for col in range(len(label_map_from_str_to_num))] for row in range(len(label_map_from_str_to_num))]
for root, _, filenames in os.walk('orig_test'):
    if len(filenames) > 0:
        for filename in filenames:
            # print(filename)
            img = cv2.imread(root+ '/'+ filename)
            channels = cv2.split(img)
            hist_of_detect = []
            for (channels, color) in zip(channels, colors):
                hist = cv2.calcHist([channels], [0], None, [256], [0, 256])
                bin_hist = []
                for i in range(binsize):
                    sigma = 0
                    for j in range(sum_len*i, sum_len*(i+1)):
                        sigma += hist[j][0]
                    bin_hist.append(sigma)
                hist_of_detect.extend(bin_hist)
            pic_label = knn(hist_vec_len, hist_vec, hist_of_detect, class_vec, 11)
            # else:
            #     pic_label = -1
            # true_positive, false_positive, false_negative statistics
            orig_label = label_map_from_str_to_num.index(filename.split('_',1)[0])
            conf_mat_te[orig_label][pic_label] += 1
            # write pic with label in filename
            # print('for pic'+filename+', label is'+str(pic_label))
            # cv2.imwrite('raw_aft/'+ filename[:-4] + '_label=' + str(pic_label) + '.jpg', img)

# training & testing precision, recall and accuracy
# diag_tr = np.array([conf_mat_tr[i][i] for i in range(len(label_map_from_str_to_num))])
# sum_tr = np.array(conf_mat_tr).sum()
# precision_tr = diag_tr/np.array(conf_mat_tr).sum(axis=0)
# recall_tr = diag_tr/np.array(conf_mat_tr).sum(axis=1)
# acc_tr = diag_tr.sum()/sum_tr

diag_te = np.array([conf_mat_te[i][i] for i in range(len(label_map_from_str_to_num))])
sum_te = np.array(conf_mat_te).sum()
precision_te = diag_te/np.array(conf_mat_te).sum(axis=0)
recall_te = diag_te/np.array(conf_mat_te).sum(axis=1)
acc_te = diag_te.sum()/sum_te

print('label sequence:' + str(label_map_from_str_to_num))
# print('training:\nprecision=' + str(precision_tr) + '\nrecall=' + str(recall_tr) + '\naccuracy=' + str(acc_tr))
# print(str(conf_mat_tr[0]) + '\n' + str(conf_mat_tr[1]) + '\n' + str(conf_mat_tr[2]) + '\n' + str(conf_mat_tr[3]) + '\n')
print('testing:\nprecision=' + str(precision_te) + '\nrecall=' + str(recall_te) + '\naccuracy=' + str(acc_te))
print(str(conf_mat_te[0]) + '\n' + str(conf_mat_te[1]) + '\n' + str(conf_mat_te[2]) + '\n' + str(conf_mat_te[3]) + '\n')