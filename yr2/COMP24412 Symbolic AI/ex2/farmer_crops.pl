:- use_module(library(clpfd)).

crops_eqs(Fields):-
	Fields = [A,B,C,D,E,F],
	A#\=B,
	A#\=C,
	A#\=D,
	A#\=F,
	B#\=C,
	B#\=D,
	C#\=D,
	C#\=E,
	D#\=E,
	D#\=F,
	E#\=F.

crops3(Fields):-
	Fields = [A,B,C,D,E,F],
	Fields ins 1..3,
	crops_eqs(Fields),
	label([A,B,C,D,E,F]).
	
crops4(Fields):-
	Fields = [A,B,C,D,E,F],
	[A,B,C,D,E,F] ins 1..4,
	crops_eqs(Fields),
	label([A,B,C,D,E,F]).