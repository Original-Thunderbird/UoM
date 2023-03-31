import cv2
from pickle import dump, load, HIGHEST_PROTOCOL

cap = cv2.VideoCapture('clip.mp4')
size = (int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)), int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))

# tracklet_stock = list(np.load('tracklets_raw.npy', allow_pickle=True))
with open('tracklets_raw.pickle', 'rb') as fr:
    tracklet_stock = load(fr)

# delete all tracklets that stays almost at the same place
delete_buffer = []
for i in range(len(tracklet_stock)):
    min_x = size[0]+1
    min_y = size[1]+1
    max_x = -1
    max_y = -1
    for detection in tracklet_stock[i][1]:
        if detection[4][0] > max_x:
            max_x = detection[4][0]
        if detection[4][0] < min_x:
            min_x = detection[4][0]
        if detection[4][1] > max_y:
            max_y = detection[4][1]
        if detection[4][1] < min_x:
            min_y = detection[4][1]
    # filter tracklet that is shorter than a threshold
    if (max_x-min_x < 10 and max_y-min_y < 10):
        delete_buffer.append(i)
if len(delete_buffer) > 0:
    delete_buffer.reverse()
    for i in range(len(delete_buffer)):
        tracklet_stock.pop(delete_buffer[i])


# label correction
for i in range(len(tracklet_stock)):
    # get all labels in tracklet
    labels = []
    for j in range(len(tracklet_stock[i][1])):
        # only use actual detection to correct label
        # if tracklet_stock[i][1][j][6] == 0:
        labels.append(tracklet_stock[i][1][j][5])
    fin_label = max(set(labels), key = labels.count)
    # assign the most common label to all the tracklet
    for j in range(len(tracklet_stock[i][1])):
        tracklet_stock[i][1][j][5] = fin_label

# remaining tracklets are sorted according to initial detection time, starting frame from small to large
length = len(tracklet_stock)
for i in range(length):
    # Last i elements are already in place
    for j in range(0, length-i-1):
        if tracklet_stock[j][0] > tracklet_stock[j+1][0] :
            tracklet_stock[j], tracklet_stock[j+1] = tracklet_stock[j+1], tracklet_stock[j]

# # # test
# for tracklet in tracklet_stock:
#     print(tracklet[0])
#     for detection in tracklet[1]:
#         print(detection)
#     print('\n\n')

# for i in range(len(tracklet_stock)):
#     for j in range(len(tracklet_stock[i][1])):
#         tracklet_stock[i][1][j][0] = int(tracklet_stock[i][1][j][0])
#         tracklet_stock[i][1][j][1] = int(tracklet_stock[i][1][j][1])
#         tracklet_stock[i][1][j][2] = int(tracklet_stock[i][1][j][2])
#         tracklet_stock[i][1][j][3] = int(tracklet_stock[i][1][j][3])


# np.save('tracklets_clear.npy', tracklet_stock)
with open('tracklets_clear.pickle', 'wb') as fw:
    dump(tracklet_stock, fw, protocol=HIGHEST_PROTOCOL)

tracklet_doubt = []
for i in range(len(tracklet_stock)):
    if tracklet_stock[i][1][0][5] == 2:
        tracklet_doubt.append(tracklet_stock[i])
# np.save('tracklets_doubt.npy', tracklet_doubt)
with open('tracklets_sin.pickle', 'wb') as fw:
    dump(tracklet_doubt, fw, protocol=HIGHEST_PROTOCOL)