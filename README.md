# UoM
Documentation of courseworks at University of Manchester

## Documented Courseworks
- [COMP24111.2 Face Classification](#2411102)
- [COMP24111.3 Spam Classification](#2411103)
- [COMP24412.3 Knowledge Representation in Metallurgy](#2441203)
- [COMP27112.2 Orrery Simulation](#2711202)
- [COMP28112.2 Hotel Booking](#2811202)

<a name="2411102"></a>
## [COMP24111.2 Face Classification]
Use KNN and linear classification for face classification. 

### 1. Getting Started
Open the project in Matlab, run task1a.m, task1b.m, task2a.m, task2b.m seperately to get result for each.

### 2. Technologies
Matlab

### 3. Tests
- task1a.m - Use KNN to classify samples of 2 classes with K in [1, 6]. Display results over 50 repetitive experiments visually and quantitatively.
- task1b.m - Use leave-one-out cross-validation to determine the best k value (for the dataset). Use KNN to classify samples of 40 classes. Calculate mean and std of accuracy over 50 repetitive experiments. 
- task2a.m - Use linear classification to classify samples of 2 classes.
- task2b.m - Use linear classification to classify samples of 40 classes. Compare results with teat1a.

In test1a and 2a, false results will have a frame added for easy identification:

<img src="yr2/COMP24111 Machine Learning and Optimisation/lab02 Face Recognition/readme-img/false-result.JPG" width="180" height="180">

while correctly classified test samples won't have it. 

### 4. Dataset
The dataset is ORLfacedata.mat which contians labeled face shots of 40 people, each person with 10 images. See 'data' and 'labels' to understand sample-label mapping.

### 5. Result
test1a.m: [plot of training accuracy versus k](yr2/COMP24111%20Machine%20Learning%20and%20Optimisation/lab02%20Face%20Recognition/readme-img/t1a_test.JPG); [plot of testing accuracy versus k](yr2/COMP24111%20Machine%20Learning%20and%20Optimisation/lab02%20Face%20Recognition/readme-img/t1a_train.JPG).

test1b.m: accuracy/standard deviation across 40 classes: 88.17%/0.0248.

test2a.m: testing accuracy over 50 rounds: 93.86%.

Result analysis is given in lab report.


<a name="2411103"></a>
## [COMP24111.3 Spam Classification]
Use Naive Bayes for Spam Recognition. Emails are preprocessed so each is represented by a vector.

### 1. Getting Started
Open the project in Matlab, run main.m. filename could be one of the following: (avc_c2.mat, av7_c3.mat, av3_c2.mat, av2_c2.mat, spambase.data).

### 2. Technologies
Matlab

### 3. Tests
 - avdis_t.m - for 3 discrete-valued datasets.
 - avcon_t.m - for continuous-valued avc_c2.mat. 
 - spam_t.m - for Spambase.data. 10-fold cross validation is conducted, visualized result is given to present result in each fold.

### 4. Dataset
 - av2_c2 - a dataset for binary classification where each attribute has two discrete values.
 - av3_c2 - a dataset for binary classification where each attribute has three discrete values.
 - av7_c3 - a dataset for three-class classification where each attribute has seven discrete values.
 - avc_c2 - a dataset for binary classification where all attributes have continuous values that are assumed to be subject to Gaussian distribution.
 - Spambase.data – [the original UCI spambase database](http://archive.ics.uci.edu/ml/datasets/Spambase).

### 5. Result
av2_c2.mat: 89.0%; av3_c2.mat: 89.2%; av7_c3.mat: 86.6%; avc_c2.mat: 76.4%; spambase.data - 81.7%. Result analysis is given in lab report.

<a name="2441203"></a>
## [COMP24412.3 Knowledge Representation in Metallurgy]
1. Translate sentences into Context Free Grammars (CFGs), Definite Clause Grammars (DFGs), Combinatory Categorical Grammars (CCGs) and NLTK lambda-calculus. Build parse tree for facts and questions.
2. Do POS-tag analysis on facts, build dependency structures, use Open Information
Extraction (OpenIE) to extract the predicate-argument structure and eventually build the knowledge base for inference and question answering.

Read Lab instruction under project directory for details.

### 1. Technologies
Prolog, [NLTK](https://github.com/nltk/nltk), [Graphene](https://github.com/Lambda-3/Graphene), [CoreNLP](http://corenlp.run/).


<a name="2711202"></a>
## [COMP27112.2 Orrery Simulation]
Generate an animated orrery with starfield as background and orbital path. Parameters for sun and each planet are given in 'sys'.

### 1. Getting Started

### 2. Technologies
OpenGL

### 3. Result
Orrery simulation:

Flying around:

Changing camera angle：


<a name="2811202"></a>
## [COMP28112.2 Hotel Booking]
We have a server that records hotel and band slots (smaller number for earlier slots), implement a client for booking hotel rooms and band slots and their number must match. Manually introduced random server down may appear. Anyone can hold at most 2 slots at the same time. Functionalities need to implement include: booking a slot (with possible error handling); cancel booking for a slot (with possible error handling); checking slots you have booked; finding free slots. Client skeleton, XML message parser and communication schema are implemented in advance.

### 1. Getting Started
not available, server is hold by the professor

### 2. Technologies
Java, XML, HTTP



[COMP24111.2 Face Classification]: <yr2/COMP24111%20Machine%20Learning%20and%20Optimisation/lab02%20Face%20Recognition>
[COMP24111.3 Spam Classification]: <yr2/COMP24111%20Machine%20Learning%20and%20Optimisation/lab03%20Spam%20Classification>
[COMP24412.3 Knowledge Representation in Metallurgy]: <yr2/COMP24412%20Symbolic%20AI/ex3>
[COMP27112.2 Orrery Simulation]: <yr2/COMP27112%20Computer%20Graphics%20and%20Image%20Processing/ex2>
[COMP28112.2 Hotel Booking]: <yr2/COMP28112%20Distributed%20Computing/ex22>

## TODO
 - pass all tests for COMP23412.2
 - build a server for COMP28112.2
 - present COMP27112.2 results in GIF
