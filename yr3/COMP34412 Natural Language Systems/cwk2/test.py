import numpy as np
import nltk
import random
from nltk import word_tokenize

# f = open('corpusA/1793-Washington.txt', "r")
# # to lower case
# s = f.read().replace('\n', ' ').lower()
# print(s)
# tokens = nltk.word_tokenize(s)
# tagged = nltk.pos_tag(tokens)
# print(tagged)


# doc_term_matrix =np.array([[ i*5+j for i in range(5)] for j in range(4)])
# print(doc_term_matrix.shape)

# l = [(0, 0), (0, 0), (1, 0), (2, 9) ,(0, 0), (1, 7), (1, 4), (2, 5), (2, 6), (0, 0), (1, 8), (1, 9)]
# m = l.index((0, 0))
# print(m)
# s = sorted(m)
# for (i, j) in s:
#     print(i)
#     print(j)

# a = ['a', 'c', 's', 'd', 'o', 's', 'b', 'a', 's']
# location_list = [index for index, word in enumerate(a) if word == 's']
# print(location_list)
# x_axix = np.array(range(2,21))
# print(x_axix)

# print([[[0] for i in range(4)] for j in range(5)])

# from os import listdir, getcwd
# print(listdir(getcwd()+'/corpus_1'))


# load dictionary
f = open('subjectivity_clues_hltemnlp05/subjclueslen1-HLTEMNLP05.tff', "r", encoding='utf-8')
dictionary = dict()
raw = f.read().split('\n')[:-1]
for line in raw:
    bar = line.split()
    # print(bar)
    word = bar[2][6:]
    # key:word -> value:strength & polarity
    if word not in dictionary:
        # print(f'{word} added')
        if bar[0][5:] == 'weaksubj':
            if bar[5][14:] == 'negative':
                dictionary[word] = -5
            elif bar[5][14:] == 'positive':
                dictionary[word] = 5
        if bar[0][5:] == 'strongsubj':
            if bar[5][14:] == 'negative':
                dictionary[word] = -10
            elif bar[5][14:] == 'positive':
                dictionary[word] = 10
print(dictionary)
negation_set = ('no', 'not', "hasn't", "haven't", "doesn't", "don't", "isn't", "wasn't", "weren't", 'never', 'seldom', 'barely', 'rarely', 
                'hardly', 'overly', 'excessively')


def read_data(source, collect, corpus, neg_switch):
    f = open(source, "r", encoding='latin-1')
    raw = f.read().split('\n')[:-1]
    if neg_switch == False:
        for rev in raw:
            words = word_tokenize(rev)
            corpus.append(words)
            for word in words:
                if word not in collect:
                    collect.append(word)

    else:
        for rev in raw:
            words = word_tokenize(rev)
            corpus.append(words)
            for word in words:
                if word not in collect and word not in negation_set:
                    collect.append(word)

collect = []

corpus = []
read_data('corpus2/rt-polarity.neg', collect, corpus, False)
read_data('corpus2/rt-polarity.pos', collect, corpus, False)
print(len(collect))
dollect = []
keys = list(dictionary.keys())
print(len(keys))
for word in collect:
    if word in keys:
        dollect.append(word)
print(len(dollect))
