from nltk.ccg import chart, lexicon
from nltk.ccg.chart import printCCGDerivation

lex = lexicon.fromstring('''
:- S, N, NP, ADJ
Which => (S/(S\\NP))/NP {\\F G w.F(w) & G(w)}
material => NP {\\v.material(v)}
has => (S\\NP)/NP {\\t s.have(s,t)}
have => (S\\NP)/NP {\\t s.have(s,t)}
the => (NP/N)/ADJ {\\x y.the(x,y)}
lowest => ADJ {lowest}
tensile => N/N {\\z.tensile(z)}
strength => N {strength}
Does => (S/(S\\NP))/NP {\\F G w.F(s) & G(s)}
Ferrite => NP {ferrite}
Perlite => NP {perlite}
Austenite => NP {austenite}
Cementite => NP {cemenite}
high => NP/N {\\r.high(r)}
medium => NP/N {\\r.medium(r)}
low => NP/N {\\r.low(r)}
hardness => N {hardness}
0_1 => NP/N {\\q.0_1(q)}
0_3 => NP/N {\\q.0_3(q)}
0_4 => NP/N {\\q.0_4(q)}
0_7 => NP/N {\\q.0_7(q)}
''', True)
parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Ferrite has 0_1 tensile strength".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Perlite has 0_3 tensile strength".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Austenite has 0_4 tensile strength".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Cementite has 0_7 tensile strength".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Ferrite has low hardness".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Perlite has medium hardness".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Austenite has medium hardness".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Which material has the lowest tensile strength".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])

parser = chart.CCGChartParser(lex, chart.DefaultRuleSet)
parses = list(parser.parse("Does Ferrite have high hardness".split()))
print(str(len(parses)) + " parses")
printCCGDerivation(parses[0])
