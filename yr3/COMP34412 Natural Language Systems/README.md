# COMP34412

### 1. Getting Started (for all scripts under this dir)
prepare python env
```
python -m venv nlp
source nlp/bin/activate
pip install -r requirements.txt
```

## COMP34412.1 POS-tagging & Distributional semantics
- POS-tagging: POS-tag inference based on word-to-tag emission probability and tag transmission probability matrix
- distributional semantics: cluster target words base on co-occurence pattern, experiment with stemming and context size. Use pseudoword disambiguation to evaluate effectiveness of the constructed feature, i.e. add reversed keywords to target list, now cluster 2n target words into n groups. If the selected feature is good enough , a word and its reverse should be clustered into the same class.


### 1. Getting Started
- POS-tagging: unzip corpusA in dataset dir, place folder corpusA under the same directory with script.
- distributional semantics: unzip corpusB and corpusC, place corresponding folder and listD under the same directory with script. It has a runtime of around 3 hours, results has been produced in the .ipynb script.

For each task, a python script(pos.py, dis_sem.py) and a jupyter notebook script(POS_tag.ipynb, distributional_semantics.ipynb) is provided. Jupyter notebook script version and python script version contains the same code, yet output is only given in jupyter notebook script.

make sure .ipynb scripts are using the environment.


### 2. Technologies
python


### 3. Result
Ways to build better feature: tweak context size, use stem instead of raw word. Detailed experiment result given in distributional_semantics.ipynb



## COMP34412.2 Named-entity recognition & Sentiment analysis
- Named-entity recognition: compare NLTK/Stanford NER
- Sentiment analysis: build a sentiment lexicon using a semi-supervised approach by bootstrapping.
  - adj_extract: determine polarity of adjs based on seed and word rules
  - review_baseline: determine sentiment by majority voting on polarity of each word & calculate accuracy
  - review_classi: detect polarity of words from polarity of given seed words and connectives like negation, but, and. Determine emotion by machine learning on bag-of-words. This takes for around 12-13 min to complete.


### 1. Getting Started
- Named-entity recognition: unzip corpus1.rar & corpus1_tagged.rar in dataset dir, place corresponding folder under the same directory with the NER.ipynb. Run NER.ipynb
- Sentiment analysis: unzip subjectivity_clues_hltemnlp05.rar and corpus2.rar, place corresponding folder under the same directory with review_baseline.ipynb & review_classi.ipynb, run the 2 scripts

make sure .ipynb scripts are using the environment.


### 2. Technologies
python


### 3. Result
Given in .ipynb scripts