member_of(X,[X|_]).
member_of(X,[_|Xs]) :-
    member_of(X,Xs).

nonmember_of(_, []).
nonmember_of(X,[Y|Ys]) :-
    dif(X,Y),
    nonmember_of(X,Ys).

contains_same([X,_],[X,_]).
contains_same([X,_],[_,X]).
contains_same([_,X],[X,_]).
contains_same([_,X],[_,X]).


father(X,Y):-
    parents(A,B,Y),
	member_of(X,[A,B]),
	man(X).

mother(X,Y):-
	parents(A,B,Y),
	member_of(X,[A,B]),
	woman(X).


sibling(X,Y):-
	dif(X,Y),
	%nonmember_of([X,Y],Acc),
	parents(A,B,X),
	parents(C,D,Y),
	contains_same([A,B],[C,D]).
	%append(Acc,[X,Y],Acc),



brother(X,Y):-
	sibling(X,Y),
	man(X).

sister(X,Y):-
	sibling(X,Y),
	woman(X).

first_cousin(X,Y):-
	dif(X,Y),
	parents(A,B,X),
	parents(C,D,Y),
	sibling(Z,W),
	member_of([Z,W],[[A,C],[A,D],[B,C],[B,D]]).

ancestor(X,Y):-
	parents(A,B,Y),
	member_of(X,[A,B]).
ancestor(X,Y):-
	father(A,Y),
	mother(B,Y),
	ancestor(Z,A),
	ancestor(W,B),
	member_of(X,[Z,W]).

cousin(X,Y):-
	dif(X,Y),
	ancestor(A,X),
	ancestor(A,Y).
