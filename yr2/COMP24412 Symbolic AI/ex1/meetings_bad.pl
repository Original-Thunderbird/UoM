is_head(X,[X|_]).

member_of(X,[X|_]).
member_of(X,[_|Xs]) :-
    member_of(X,Xs).

nonmember_of(_, []).
nonmember_of(X,[Y|Ys]) :-
    dif(X,Y),
    nonmember_of(X,Ys).

student_pair([X,Y]):-
    student(X),
    student(Y).
     

alldifferent([ ]).
alldifferent([X|Xs]):-
    nonmember_of(X,Xs),
    alldifferent(Xs).

student(student6).
student(student5).
student(student4).
student(student3).
student(student2).
student(student1).

larger_than(student6,student5).
larger_than(student6,student4).
larger_than(student6,student3).
larger_than(student6,student2).
larger_than(student6,student1).
larger_than(student5,student4).
larger_than(student5,student3).
larger_than(student5,student2).
larger_than(student5,student1).
larger_than(student4,student3).
larger_than(student4,student2).
larger_than(student4,student1).
larger_than(student3,student2).
larger_than(student3,student1).
larger_than(student2,student1).

meetings_one_two_three( A-B, C-D, E-F ) :-
% A-F are students
student(A), student(B), student(C), student(D), student(E), student(F), 
% A-F are different
alldifferent( [ A, B, C, D, E, F ] ),
% student1 must meet in slot1
is_head( [ X, student1 ], [ [A, B], [C, D], [E, F] ] ),
student( X ),
dif( X, student1 ),
% student2 and student3 must meet in the same slot
member_of( [ student3, student2 ], [ [C, D], [E, F] ] ),
% student1 and student4 cannot meet in the same slot
nonmember_of( student4, [ X, student1 ] ),
student( X ),
% student6 cannot meet in slot1 or 'slot4'
member_of( student6, [ C, D, E, F ] ),
% sequence in slot:
larger_than(A,B),
larger_than(C,D),
larger_than(E,F),
% bad constraint:
nonmember_of( [ student3, student2 ], [ [C, D], [E, F] ] ).
