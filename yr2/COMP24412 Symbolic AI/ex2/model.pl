gives(alpha,log_mod)
gives(bravo,pro_mic).
gives(charlie,sof_eng).
gives(delta,mac_opt).
gives(echo,ope_sys).
gives(golf,sys_arc).
gives(hotel,alg_imp).
gives(hotel,sym_ai).
gives(india,com_gra).
gives(juliet,dis_com).
gives(kilo,mob_sys).
gives(lima,com_gra).
gives(mike,sof_eng).
gives(golf,ope_sys).
gives(november,alg_imp).
gives(oscar,com_gra).

chooses(a,log_mod).
chooses(a,sof_eng).
chooses(a,ope_sys).
chooses(a,com_gra).
chooses(be,pro_mic).
chooses(be,ope_sys).
chooses(be,sys_arc).
chooses(ve,alg_imp).
chooses(ve,com_gra).
chooses(ge,mob_sys).
chooses(de,log_mod).
chooses(de,mob_sys).
chooses(dje,pro_mic).
chooses(dje,com_gra).
chooses(dje,mob_sys).
chooses(gje,ope_sys).
chooses(gje,sof_eng).
chooses(ye,alg_imp).

course_unit(log_mod).
course_unit(pro_mic).
course_unit(sof_eng).
course_unit(mac_opt).
course_unit(sym_ai).
course_unit(ope_sys).
course_unit(sys_arc).
course_unit(alg_imp).
course_unit(com_gra).
course_unit(dis_com).
course_unit(mob_sys).

teacher(alpha).
teacher(bravo).
teacher(charlie).
teacher(delta).
teacher(echo).
teacher(foxtrot).
teacher(golf).
teacher(hotel).
teacher(india).
teacher(juliet).
teacher(kilo).
teacher(lima).
teacher(mike).
teacher(november).
teacher(oscar).

student(a).
student(be).
student(ve).
student(ge).
student(de).
student(dje).
student(gje).
student(ye).

friend(a,be).
friend(be,ve).
friend(ve,ge).
friend(ge,de).
friend(de,dje).
friend(dje,gje).
friend(gje,ye).
friend(ye,be).

older_than(alpha,charlie).
older_than(alpha,golf).
older_than(alpha,lima).
older_than(bravo,echo).
older_than(bravo,india).
older_than(bravo,lima).
older_than(charlie,delta).
older_than(charlie,hotel).
older_than(charlie,november).
older_than(delta,lima).
older_than(delta,oscar).
older_than(echo,kilo).

same_age(alpha,bravo).
same_age(charlie,echo).
same_age(golf,juliet).
same_age(lima,oscar).

soft(X):- chooses(X,log_mod).
soft(X):- chooses(X,sof_eng).
soft(X):- chooses(X,mac_opt).
soft(X):- chooses(X,sym_ai).
soft(X):- chooses(X,alg_imp).
soft(X):- chooses(X,com_gra).

hard(X):- chooses(X,pro_mic).
hard(X):- chooses(X,ope_sys).
hard(X):- chooses(X,sys_arc).
hard(X):- chooses(X,dis_com).
hard(X):- chooses(X,mob_sys).

classmate(X,Y):-
	chooses(X,Z),
	chooses(Y,Z),
	dif(X,Y).
	
colleague(X,Y):-
	gives(X,Z),
	gives(Y,Z),
	dif(X,Y).
	
teach(X,Y):-
	gives(X,Z),
	chooses(Y,Z).
	
hybrid(X):-
	soft(X),
	hard(X).

acquaintance(X,Y):-
	friend(X,Y).
acquaintance(X,Y):-
	friend(X,Z),
	acquaintance(Z,Y).
	
older(X,Y):-
	older_than(X,Y).
older(X,Y):-
	older_than(X,Z),
	older_than(Z,Y).
older(X,Y):-
	same_age(X,Z),
	older_than(Z,Y).
	
heardof(X,Y):-
	teach(X,Z),
	acquaintance(Z,Y).
	
younger(X,Y):-
	older(Y,X).