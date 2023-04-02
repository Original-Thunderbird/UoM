import re
import nltk

Bart_the_Fink_path = 'Bart_the_Fink.txt'
Bart_the_Lover_path = 'Bart_the_Lover.txt'
Bart_the_General_path = 'Bart_the_General.txt'
Bart_the_Mother_path = 'Bart_the_Mother.txt'
Bart_the_Genius_path = 'Bart_the_Genius.txt'
Bart_the_Murderer_path = 'Bart_the_Murderer.txt'
log_path = 'log.txt'

with open(Bart_the_Fink_path,'r') as f0:
    fink = f0.readlines()
with open(Bart_the_Lover_path,'r') as f1:
    lover = f1.readlines()
with open(Bart_the_General_path,'r') as f2:
    general = f2.readlines()
with open(Bart_the_Mother_path,'r') as f3:
    mother = f3.readlines()
with open(Bart_the_Genius_path,'r') as f4:
    genius = f4.readlines()
with open(Bart_the_Murderer_path,'r') as f5:
    murderer = f5.readlines()
with open(log_path,'r') as f5:
    log = f5.readlines()

def lines_to_map(ls):
    temp_map = {}
    row = 0
    for line in ls:
        row += 1
        col = 0
        token_list = nltk.word_tokenize(line.lower())
        for token in token_list:
            col += 1
            temp_map[str(row)+','+str(col)] = token
    return temp_map

map_fink = lines_to_map(fink)
map_lover = lines_to_map(lover)
# print(map_lover)
map_general = lines_to_map(general)
map_mother = lines_to_map(mother)
map_genius = lines_to_map(genius)
# print(map_genius)
map_murderer = lines_to_map(murderer)

# number of indices in log
total = 0
# number of indices from log.txt that are at the indicated place in corresponding txt file
count = 0
thr = 0
# records words that are not found by coord and doc name in log.txt
miss_map = {}

for line in log:
    if line.find('Bart')>=0:
        doc = line.split()[0]
        indices = re.findall(r"[0-9],[0-9]+", line)
        if Bart_the_Fink_path in doc:
            for index in indices:
                total += 1
                if word in map_fink[index]:
                    count += 1
                else:
                    miss_map[index] = (doc, word[0])
        elif Bart_the_Lover_path in doc:
            for index in indices:
                total += 1
                if word == map_fink[index]:
                    count += 1
                else:
                    miss_map[index] = (doc, word[0])
        elif Bart_the_General_path in doc:
            for index in indices:
                total += 1
                if word == map_fink[index]:
                    count += 1
                else:
                    miss_map[index] = (doc, word[0])
        elif Bart_the_Mother_path in doc:
            for index in indices:
                total += 1
                if word == map_fink[index]:
                    count += 1
                else:
                    miss_map[index] = (doc, word[0])
        elif Bart_the_Genius_path in doc:
            for index in indices:
                total += 1
                if word == map_fink[index]:
                    count += 1
                else:
                    miss_map[index] = (doc, word[0])
        elif Bart_the_Murderer_path in doc:
            for index in indices:
                total += 1
                if word == map_fink[index]:
                    count += 1
                else:
                    miss_map[index] = (doc, word[0])
    elif line == '\n':
        continue
    else:
        word = line.split()[0]
        # print(word)
    # thr += 1
    # if thr >= 30:
    #     break

# for a correctly produced log.txt, count == total, miss_map should be empty
print(total)
print(count)
print(miss_map)

# print(log[1])
# print(log[1].split()[0])
# print(re.findall(r"[0-9],[0-9]+", log[4]))