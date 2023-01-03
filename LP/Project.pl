% ist1106336, Enzo Nunes Pedralho :)
:- set_prolog_flag(answer_write_options,[max_depth(0)]). % para listas completas
:- ['dados.pl'], ['keywords.pl']. % ficheiros a importar.

eventosSemSalas(EventosSemSala) :-
    findall(E, evento(E,_,_,_,semSala), EventosSemSala).

eventosSemSalasDiaSemana(DiaDaSemana, EventosSemSala) :-
    findall(E, (evento(E,_,_,_,semSala), horario(E,DiaDaSemana,_,_,_,_)), EventosSemSala).

eventosSemSalasPeriodo(ListaPeriodos, EventosSemSala) :-
    adicionaSemestres(ListaPeriodos, ListaNova),
    findall(E, (evento(E,_,_,_,semSala), horario(E,_,_,_,_,P), member(P, ListaNova)), EventosSemSala).

% Predicado auxiliar para adicionar os semestres 1_2 e 3_4 à ListaPeriodos.
adicionaSemestres(ListaPeriodos, ListaNova) :-
    member(p1, ListaPeriodos),                                 ListaNova = [p1_2|ListaPeriodos], !;
    member(p2, ListaPeriodos), not(member(p1, ListaPeriodos)), ListaNova = [p1_2|ListaPeriodos], !;
    member(p3, ListaPeriodos),                                 ListaNova = [p3_4|ListaPeriodos], !;
    member(p4, ListaPeriodos), not(member(p3, ListaPeriodos)), ListaNova = [p3_4|ListaPeriodos], !.
