import cv2 
import numpy as np
import os

# bag of words construction, and record example and label it belong to for each feature vector
word_bag_size = 75 #hyperparam, each cluster is a word in BOVW model
ROI_type = 'MSER'
SIFT_TRUE_MSER_FALSE = False
# ROI_type = 'SIFT'
# SIFT_TRUE_MSER_FALSE = True

player_label = 0
feat_vecs = [] #len*128 after collection
label_vecs = []
label_map_from_str_to_num = []
#collect SIFT feature vectors
for root, _, filenames in os.walk('orig_train'):
    if len(filenames) > 0:
        label_map_from_str_to_num.append(root[11:])
        eg_num = 0
        for filename in filenames:
            # print(filename)
            img = cv2.imread(root+ '/'+ filename)
            if img is None:
                print('for' + filename + ',img is none')
            sift = cv2.xfeatures2d_SIFT.create()
            # kp: location
            if SIFT_TRUE_MSER_FALSE == True:
                kp, des = sift.detectAndCompute(img,None)
            else:
                mser = cv2.MSER_create()
                kp1 = mser.detect(img)
                kp2, des = sift.compute(img, kp1)
            # des: feature vectors, element in des already in float32
            if des is not None:
                feat_vecs.extend(des)
                # print(len(des))
                label_vecs.extend([[player_label, eg_num]]*len(des))
            # else:
            #     print('for' + filename + ',des is none')
            eg_num += 1
        player_label += 1
# print(feat_vecs[0])
# print(len(label_vecs))
# print(len(feat_vecs)) #74393
feat_vecs = np.float32(feat_vecs)
# Define criteria = ( type, max_iter = 100 , epsilon = 1.0 )
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 100, 0.01)
# Set flags (Just to avoid line break in the code)
flags = cv2.KMEANS_PP_CENTERS
# Apply KMeans
# centers are ordered in label indices. e.g. centers[0] is center for points with label 0, centers[1] is center for points with label 1……
compactness,labels,centers = cv2.kmeans(feat_vecs, word_bag_size, None, criteria, 10, flags)
# print(labels)


# sow examples in bovw vector space
eg_num = -1
# only for usage of this var in 'hist_vec.append(histogram)'
histogram = []
player_label = -1
hist_vec = []
class_vec = []
for i in range(len(feat_vecs)):
    # initialize a new histogram vector
    if label_vecs[i][1] != eg_num:
        if i > 0:
            hist_vec.append(histogram)
            class_vec.append(player_label)
        eg_num = label_vecs[i][1]
        player_label = label_vecs[i][0]
        histogram = [0]*word_bag_size
    # index is in form of labels[i][0] instead of labels[i] because labels[i] is a list with 1 element
    histogram[labels[i][0]] += 1
hist_vec.append(histogram)
class_vec.append(player_label)
np.savez(ROI_type+'_model.npz', hist_vec=hist_vec, class_vec=class_vec, centers = centers, label_map_from_str_to_num = label_map_from_str_to_num)