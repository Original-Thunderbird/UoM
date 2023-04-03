# COMP30040 Identifying Players in Broadcast Sports Videos
The identification system consists of 4 major parts: detection, identification, tracking, tracklet cleaning and label correction.
 - Detection: Tried with background subtraction, SVM with HOG descriptor and Mask-RCNN. Proposed a heuristic to deal with difficulty when partial occlusion occurs.
 - Identification: Tried with SIFT, MSER, RGB and their combinations
 - Tracking: Follows the idea of tracking-by-detection, Kalman filter used to take both prediction and actual detection into consideration. Proposed a heuristic to predict players' position under severe occlusion.
 - Cleaning/Label Correction: discard tracklets (a detection sequence of the same player) that are too short; run label majority voting within tracklets


## 0. Background
Figure tracking has been a widely investigated problem and there are many satisfying results for pedestrian tracking. However, research of tracking for
sports players is not that well extended as it is more difficult due to unpredictable trajectories. Low quality of players also poses a challenge, as it limits number of features of each figure and brings difficulty to identification.

Upon a fully functional release of such a system, Coaches can better analyze players' performance, e.g. total running distance, interceptions, pressing and make tactics with support of fresh data.


## 1. Getting started
### Environment
```
python -m venv tracker
source tracker/bin/activate
pip install numpy
pip install opencv-python
```
### Project Pipeline
1. build MSER+SIFT bag-of-words model
```
python SIFT_build.py
```
output MSER_model.npz/SIFT_model.npz contains model parameters

2. build tracklets
```
python track_gather.py (input video name)
```
loads MSER_model.npz/SIFT_model.npz for player classification, output tracklets_raw.pickle that contains info of all generated tracklets (with no identity assigned at this time)

3. tracklet cleaning & label assignment
```
python track_clean.py
```
loads tracklets_raw.pickle, outputs tracklets_clear.pickle for drawing & tracklets_sin.pickle for debugging

4. draw tracklet on video
```
python track_draw.py (input video name) (output video name)
```
loads tracklets_clear.pickle/tracklets_sin.pickle, single-frame debugging if tracklets_sin.pickle used; else draw tracklets on your input video (should be same as that used with track_gather.py) from tracklets_clear.pickle and output result.

### Miscellaneous
 - detect_bs.py: detect players by background subtraction, save them in raw/
  ```
  python detect_bs.py (video name)
  ```
 - detect_svm.py: detect players by SVM with HOG descriptor, draw bounding box in output video
```
python detect_svm.py (input video name) (output video name)
```
 - SIFT_test.py: produce confusion matrix on training set & test set with result gathered from SIFT_build.py
```
python SIFT_test.py
```
confusion matrix will be given in terminal

 - RGB_test.py: produce confusion matrix on training set & test set with RGB bin histogram feature
```
python RGB_test.py
```
confusion matrix will be given in terminal


## 2. Technologies
python, openCV, SVM, Faster-RCNN, SIFT, MSER, kalman filter


## 3. Dataset
Contact dafu690127@gmail.com


## 4. Result
See quantified test result in report part 4. 
### 3.1 Detection
<table>
    <tr>
        <td><a href="https://youtu.be/E42bov8eKG4">Background Subtraction</a></td>
        <td><a href="https://youtu.be/NFJGG6RujWI">SVM on HOG descriptor</a></td>
        <td><a href="https://youtu.be/s_j1o5R5tOo">Mask RCNN</a></td>
        <td><a href="https://youtu.be/zu8DvtoVZTs">Mask RCNN with heuristic</a></td>
    </tr>
</table>
problem with background subtraction: fail easily when players are close;

problem with SVM+HOG: low accuracy with lots of false positives (see report) and false negatives (see video above)

### 3.2 Tracking result
A [short sample](https://youtu.be/91GWq3lQm2Q) and a [longer video](https://youtu.be/CocZy02Ttpw); with final [screencast](https://youtu.be/AA-iH8KUEHI)

## 4. Possbile further improvements
1. There are cases that multiple players identified as the same person. In this case we may give scores to multiple detections sharing the same label and only use the one with highest scores.
2. Since the task only focus on players (and the football at most), We can fine tune the neural network model on a dataset of only players and football.
3. In case players wear their jersey, the number on jersey and team of each color may provide extra info.