isa_list([]).
isa_list([_Head|Tail]):-
	isa_list(Tail).

nonmember_of(_, []).
nonmember_of(X,[Y|Ys]) :-
    dif(X,Y),
    nonmember_of(X,Ys).

is_pos(south).
is_pos(north).
side_opposite(X,Y):-
	dif(X,Y),
	is_pos(X),
	is_pos(Y).

wfgc(F,W,G,C):-
	is_pos(W),
	is_pos(F),
	is_pos(G),
	is_pos(C).

is_state(wfgc(F,W,G,C)):-
	is_pos(W),
	is_pos(F),
	is_pos(G),
	is_pos(C).


move(wfgc(X,X,G,C),wfgc(Y,Y,G,C),wolf):-
	side_opposite(X,Y).
move(wfgc(X,W,X,C),wfgc(Y,W,Y,C),goat):-
	side_opposite(X,Y).
move(wfgc(X,W,G,X),wfgc(Y,W,G,Y),cabbage):-
	side_opposite(X,Y).
move(wfgc(X,W,G,C),wfgc(Y,W,G,C),alone):-
	side_opposite(X,Y).

safestate(wfgc(X,_,X,_)).
safestate(wfgc(X,X,_,X)).

%safestate(wfgc(F,W,G,C)):-
%	(F = north, W = north, G = north, C = north);
%	(F = south, W = south, G = south, C = south);
%	(F = G,
%	side_opposite(G,W),
%	side_opposite(G,C));
%	(F = W,
%	 F = C,
%	side_opposite(G,W),
%	side_opposite(G,C)).

puzzlestate_moves(wfgc(south,south,south,south),[]).
puzzlestate_moves(CState,[M|Ms]):-
	move(CState,PState,M),
	safestate(PState),
	puzzlestate_moves(PState, Ms).

puzzlestate_moves_without(wfgc(south,south,south,south),[],Acc):-
	nonmember_of(wfgc(south,south,south,south),Acc).
puzzlestate_moves_without(CState,[M|Ms],Acc):-
	nonmember_of(CState,Acc),
	move(CState,PState,M),
	safestate(PState),
	puzzlestate_moves_without(PState, Ms, [CState|Acc]).
