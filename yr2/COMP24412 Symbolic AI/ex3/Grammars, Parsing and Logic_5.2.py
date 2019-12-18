from nltk.ccg import chart, lexicon
from nltk.ccg.chart import printCCGDerivation

lex = lexicon.fromstring('''
:- S, NP, N
Steel => NP
is => (S\\NP)/NP
an => NP/N
alloy => N
''', False)
parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Steel is an alloy".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

lex = lexicon.fromstring('''
:- S, NP, N
Steel => NP
contains => (S\\NP)/NP
carbon => NP
''', False)
parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Steel contains carbon".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

lex = lexicon.fromstring('''
:- S, NP, N
Does => (S/(S\\NP))/N
Ferrite => N
have => (S\\NP)/NP
high => NP/N
hardness =>N
''', False)
parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Does Ferrite have high hardness".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

lex = lexicon.fromstring('''
:-S, NP, N, ADJ
Which => (S/(S\\NP))/N
material => N
has => (S\\NP)/NP
the => (NP/N)/ADJ
lowest => ADJ
tensile => N/N
strength => N
''', False)
parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Which material has the lowest tensile strength".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

lex = lexicon.fromstring('''
:- S, NP, N
Steel => NP {steel}
is => (S\\NP)/NP {\\x y.be(y, x)}
an => NP/N {\\P.exists z.P(z)}
alloy => N {alloy}
''', True)
parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Steel is an alloy".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

lex = lexicon.fromstring('''
:- S, NP, N
Steel => NP {steel}
contains => (S\\NP)/NP {\\x y.contain(y, x)}
carbon => NP {carbon}
''', True)
parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Steel contains carbon".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

lex = lexicon.fromstring('''
:- S, NP, N
Does => (S/(S\\NP))/N {\\F G w.F(w) & G(w)}
Ferrite => N {ferrite}
have => (S\\NP)/NP {\\y z.have(z,y)}
high => NP/N {\\x.high(x)}
hardness => N {hardness}
''', True)
parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Does Ferrite have high hardness".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

lex = lexicon.fromstring('''
:- S, NP, N, ADJ
Which => (S/(S\\NP))/N {\\F G w.F(w) & G(w)}
material => N {\\v.material(v)}
has => (S\\NP)/NP {\\t u.have(u,t)}
the => (NP/N)/ADJ {\\x y.the(y,x)}
lowest => ADJ {lowest}
tensile => N/N {\\z.tensile(z)}
strength => N {strength}
''', True)
parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Which material has the lowest tensile strength".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])
