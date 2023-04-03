Bart_the_Fink_path = 'Bart_the_Fink.txt'
Bart_the_Lover_path = 'Bart_the_Lover.txt'
Bart_the_General_path = 'Bart_the_General.txt'
Bart_the_Mother_path = 'Bart_the_Mother.txt'
Bart_the_Genius_path = 'Bart_the_Genius.txt'
Bart_the_Murderer_path = 'Bart_the_Murderer.txt'
stopword_list_path = 'stopwords.txt'

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
    
with open(stopword_list_path, 'r') as f6:
    stopwords = f6.readlines()
stopwords = [x.strip() for x in stopwords]

import re
from nltk.stem.porter import *

stemmer = PorterStemmer()
dictionary = dict()

doc_names = ['Bart_the_Fink.txt.gz', 'Bart_the_Lover.txt.gz', 'Bart_the_Mother.txt.gz', 'Bart_the_General.txt.gz', 'Bart_the_Genius.txt.gz', 'Bart_the_Murderer.txt.gz']

reg_tmp = re.compile('^\d+$|^.$|(^[0-9][a-z][0-9][0-9]$)')

# for doc_index, doc in enumerate(fink_reg):
for doc_index, doc in enumerate([fink, lover, mother, general, genius, murderer]):
    for para_index, paragraph in enumerate(doc):
        paragraph = paragraph.replace('\[\d\]','')
        paragraph = paragraph.lower().split()
        
        for word_index, raw in enumerate(paragraph):
            if 'http' in raw:
                raw = raw.replace('\"','')
            else:
                raw = re.sub('[^a-zA-Z0-9-]', '', raw)
            raw = re.sub('^\d+$|^.$|(^[0-9][a-z][0-9][0-9]$)', '', raw)
            if raw == '':
                continue
            # stopword removal
            if raw in stopwords:
                continue
            text = stemmer.stem(raw)
            if text in dictionary:
                if doc_names[doc_index] in dictionary[text]:
                    dictionary[text][doc_names[doc_index]].append([para_index+1, word_index+1])
                else:
                    dictionary[text][doc_names[doc_index]] = [[para_index+1, word_index+1]]
            else:
                dictionary[text] = dict()
                dictionary[text][doc_names[doc_index]] = [[para_index+1, word_index+1]]

sorted_dict = dict(sorted(dictionary.items()))

output_path = 'output.txt'
with open(output_path, "w") as text_file:
    for key, value in sorted_dict.items():
        print(key, file=text_file)
        for val_key, val_value in value.items():
            print(val_key, file=text_file, end="")
            print(val_value, file=text_file)
        print('', file=text_file)