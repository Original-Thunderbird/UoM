% the three list predicates mentioned in the slides

isa_list([]).
isa_list([_X|Xs]) :-
    isa_list(Xs).

member_of(X,[X|_]).
member_of(X,[_|Xs]) :-
    member_of(X,Xs).

nonmember_of(_, []).
nonmember_of(X,[Y|Ys]) :-
    dif(X,Y),
    nonmember_of(X,Ys).


alldifferent([]).
alldifferent([X|Xs]):-
    nonmember_of(X,Xs),
    alldifferent(Xs).

length_two([_,_]).