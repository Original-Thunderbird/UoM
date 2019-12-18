fof(f01,axiom, course_unit(log_mod)).
fof(f02,axiom, course_unit(pro_mic)).
fof(f03,axiom, course_unit(sof_eng)).
fof(f04,axiom, course_unit(mac_opt)).
fof(f05,axiom, course_unit(sym_ai)).
fof(f06,axiom, course_unit(ope_sys)).
fof(f07,axiom, course_unit(sys_arc)).
fof(f08,axiom, course_unit(alg_imp)).
fof(f09,axiom, course_unit(com_gra)).
fof(f10,axiom, course_unit(dis_com)).
fof(f11,axiom, course_unit(mob_sys)).
fof(distinct,axiom, $distinct(log_mod,pro_mic,sof_eng,mac_opt,sym_ai,ope_sys,sys_arc,alg_imp,com_gra,dis_com,mob_sys)).

fof(f12,axiom, teacher(alpha)).
fof(f13,axiom, teacher(hotel)).
fof(f14,axiom, teacher(bravo)).
fof(f15,axiom, teacher(charlie)).
fof(f16,axiom, teacher(delta)).
fof(f17,axiom, teacher(echo)).
fof(f18,axiom, teacher(foxtrot)).
fof(f19,axiom, teacher(golf)).
fof(f20,axiom, teacher(india)).
fof(f21,axiom, teacher(juliet)).
fof(f22,axiom, teacher(kilo)).
fof(f23,axiom, teacher(lima)).
fof(f24,axiom, teacher(mike)).
fof(f25,axiom, teacher(november)).
fof(f26,axiom, teacher(oscar)).
fof(distinct,axiom, $distinct(alpha,bravo,charlie,delta,echo,foxtrot,golf,india,juliet,kilo,lima,mike,november,oscar)).

fof(f27,axiom, student(a)).
fof(f28,axiom, student(be)).
fof(f29,axiom, student(ve)).
fof(f30,axiom, student(ge)).
fof(f31,axiom, student(de)).
fof(f32,axiom, student(dje)).
fof(f33,axiom, student(gje)).
fof(f34,axiom, student(ye)).
fof(distinct,axiom, $distinct(a,be,ve,ge,de,dje,gje,ye)).

fof(f34,axiom, gives(alpha,log_mod)).
fof(f35,axiom, gives(bravo,pro_mic)).
fof(f36,axiom, gives(charlie,sof_eng)).
fof(f37,axiom, gives(delta,mac_opt)).
fof(f38,axiom, gives(echo,ope_sys)).
fof(f39,axiom, gives(golf,sys_arc)).
fof(f40,axiom, gives(hotel,alg_imp)).
fof(f41,axiom, gives(hotel,sym_ai)).
fof(f42,axiom, gives(india,com_gra)).
fof(f43,axiom, gives(juliet,dis_com)).
fof(f44,axiom, gives(kilo,mob_sys)).
fof(f45,axiom, gives(lima,com_gra)).
fof(f46,axiom, gives(mike,sof_eng)).
fof(f47,axiom, gives(golf,ope_sys)).
fof(f48,axiom, gives(november,alg_imp)).
fof(f49,axiom, gives(oscar,com_gra)).

fof(f50,axiom, chooses(a,log_mod)).
fof(f51,axiom, chooses(a,sof_eng)).
fof(f52,axiom, chooses(a,ope_sys)).
fof(f53,axiom, chooses(a,com_gra)).
fof(f54,axiom, chooses(be,pro_mic)).
fof(f55,axiom, chooses(be,ope_sys)).
fof(f56,axiom, chooses(be,sys_arc)).
fof(f57,axiom, chooses(ve,alg_imp)).
fof(f58,axiom, chooses(ve,com_gra)).
fof(f59,axiom, chooses(ge,mob_sys)).
fof(f60,axiom, chooses(de,log_mod)).
fof(f61,axiom, chooses(de,mob_sys)).
fof(f62,axiom, chooses(dje,pro_mic)).
fof(f63,axiom, chooses(dje,com_gra)).
fof(f64,axiom, chooses(dje,mob_sys)).
fof(f65,axiom, chooses(gje,ope_sys)).
fof(f66,axiom, chooses(gje,sof_eng)).
fof(f67,axiom, chooses(ye,alg_imp)).

fof(f87,axiom, older_than(alpha,charlie)).
fof(f88,axiom, older_than(alpha,golf)).
fof(f89,axiom, older_than(alpha,lima)).
fof(f90,axiom, older_than(bravo,echo)).
fof(f91,axiom, older_than(bravo,india)).
fof(f92,axiom, older_than(bravo,lima)).
fof(f93,axiom, older_than(charlie,delta)).
fof(f94,axiom, older_than(charlie,hotel)).
fof(f95,axiom, older_than(charlie,november)).
fof(f96,axiom, older_than(delta,lima)).
fof(f97,axiom, older_than(delta,oscar)).
fof(f98,axiom, older_than(echo,kilo)).

fof(f99,axiom, same_age(alpha,bravo)).
fof(f100,axiom, same_age(charlie,echo)).
fof(f101,axiom, same_age(golf,juliet)).
fof(f102,axiom, same_age(lima,oscar)).

fof(f68,axiom, friend(a,be)).
fof(f69,axiom, friend(be,ve)).
fof(f70,axiom, friend(ve,ge)).
fof(f71,axiom, friend(ge,de)).
fof(f72,axiom, friend(de,dje)).
fof(f73,axiom, friend(dje,gje)).
fof(f74,axiom, friend(gje,ye)).
fof(f75,axiom, friend(ye,be)).

fof(f76,axiom, ![X] : ( ( chooses(X,log_mod) | chooses(X,sof_eng) | chooses(X,mac_opt) | chooses(X,sym_ai) | chooses(X,alg_imp) | chooses(X,com_gra) ) => soft(X) ) ).

fof(f77,axiom, ![X] : ( ( chooses(X,pro_mic) | chooses(X,ope_sys) | chooses(X,sys_arc) | chooses(X,dis_com) | chooses(X,mob_sys) ) => hard(X) ) ).

fof(f78,axiom, ![X,Y] : ( ?[Z]:( chooses(X,Z) & chooses(Y,Z) & X!=Y ) => classmate(X,Y) ) ).

fof(f79,axiom, ![X,Y] :( ?[Z]: ( gives(X,Z) & gives(Y,Z) & X!=Y )  => colleague(X,Y) ) ).

fof(f80,axiom, ![X,Y] : ( ?[Z]: ( gives(X,Z) & chooses(Y,Z) )  => teach(X,Y) ) ).

fof(f81,axiom, ![X] : ( ( soft(X) & hard(X) ) => hybrid(X) ) ).

fof(f82,axiom, ![X] : ( ![Y] : ( ( friend(X,Y) | ?[Z] : ( friend(X,Z) & acquaintance(Z,Y) ) ) => acquaintance(X,Y) ) ) ).

fof(f84,axiom, ![X,Y] : ( ( older_than(X,Y) | ?[Z] : ( ( older_than(X,Z) | same_age(X,Z) ) & older_than(Z,Y) ) ) => older(X,Y) ) ).

fof(f85,axiom, ![X,Y]: ( ?[Z] : ( teach(X,Z) & acquaintance(Z,Y) ) => heardof(X,Y) ) ).

fof(f86,axiom, ![X,Y] : ( older(X,Y) => younger(Y,X) ) ).
