import nltk
import os
from collections import Counter

# get corpus & its size
corpus = []
size = 0
for root, _, filenames in os.walk('corpusA'):
    for filename in filenames:
        f = open('corpusA/'+filename, "r", encoding='latin-1')
        # to lower case
        s = f.read().replace('\n', ' ').lower()
        tokens = nltk.word_tokenize(s)
        tagged = nltk.pos_tag(tokens)
        corpus.append(tagged)
        size += len(tagged)


# calculate emission possibility
# collect tags
tag_list = []
for corpora in corpus:
    for word in corpora:
        if word[1] not in tag_list:
            tag_list.append(word[1])

print(tag_list)
print(size)

# classify word to tag_list list
word_collect = []
for i in range(len(tag_list)):
    word_collect.append([])
for corpora in corpus:
    for i in range(len(corpora)):
        index = tag_list.index(corpora[i][1])
        word_collect[index].append(corpora[i][0])

# calculate word possibility & tag_list probability
word_possibility = []
tag_count = []
tag_probability = []
for word_collect_on_tag in word_collect:
    tag_probability.append(len(word_collect_on_tag)/size)
    tag_count.append(len(word_collect_on_tag))
    word_possibility_on_tag = []
    cnt = Counter(word_collect_on_tag)
    l = len(word_collect_on_tag)
    for key in cnt:
        word_possibility_on_tag.append([key, cnt[key]/l])
    word_possibility.append(word_possibility_on_tag)

for i in range(len(word_possibility)):
    print(tag_list[i])
    print(word_possibility[i])

# calculate transition probility
transition_probability = []
for i in range(len(tag_list)):
    transition_probability.append([0]*len(tag_list))

for corpora in corpus:
    for i in range(1, len(corpora)):
        fore = tag_list.index(corpora[i-1][1])
        aft = tag_list.index(corpora[i][1])
        transition_probability[fore][aft] += 1

# for i in range(len(tag_list)):
#     if sum(transition_probability[i]) != tag_count[i]:
#         print('tag_list: '+str(tag_list[i])+' count: '+str(tag_count[i])+' sum: '+ str(sum(transition_probability[i])))

for i in range(len(tag_list)):
    for j in range(len(tag_list)):
        transition_probability[i][j] /= tag_count[i]

# for i in range(len(tag_list)):
#     if sum(transition_probability[i]) != 1:
#         print('tag_list: '+str(tag_list[i])+' sum: '+ str(sum(transition_probability[i])))

NN_index = tag_list.index('NN')
VB_index = tag_list.index('VB')
DT_index = tag_list.index('DT')
IN_index = tag_list.index('IN')

race_NN = 0
for word in word_possibility[NN_index]:
    if word[0] == 'race':
        race_NN = word[1]
# print(race_NN)
race_VB = 0
for word in word_possibility[VB_index]:
    if word[0] == 'race':
        race_VB = word[1]
# print(race_VB)
NN_DT = transition_probability[DT_index][NN_index]
# print(NN_DT)
VB_DT = transition_probability[DT_index][VB_index]
# print(VB_DT)
for_IN = 0
for word in word_possibility[IN_index]:
    if word[0] == 'for':
        for_IN = word[1]
IN_NN = transition_probability[NN_index][IN_index]
# print(IN_NN)
IN_VB = transition_probability[VB_index][IN_index]
# print(IN_VB)
# p1 = P(race|NN) * P(NN|DT) *P(IN|NN)
p1 = race_NN * NN_DT * IN_NN
# p1 = P(race|VB) * P(VB|DT) *P(IN|VB)
p2 = race_VB * VB_DT * IN_VB
print('possibility that word \'race\' has tag NN is :'+str(p1))
print('possibility that word \'race\' has tag VB is :'+str(p2))
# for i in range(len(tag_list)):
#     print(str(tag_list[i])+' '+str(tag_count[i]))
