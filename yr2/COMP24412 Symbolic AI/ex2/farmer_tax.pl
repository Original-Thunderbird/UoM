:- use_module(library(clpfd)).

cows_chicken_eqs(Cows,Chicken):-
	Cows + Chicken #= 65,
	Chicken*2 + Cows*4 #= 226.
	
cows_chicken(Cows, Chicken):-
	Cows in 0..65,
	Chicken in 0..65.