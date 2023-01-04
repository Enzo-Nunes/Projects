% ist1106336, Enzo Nunes Pedralho :)
:- set_prolog_flag(answer_write_options,[max_depth(0)]). % para listas completas
:- ['dados.pl'], ['keywords.pl']. % ficheiros a importar.

eventosSemSalas(EventosSemSala) :-
    findall(E, evento(E,_,_,_,semSala), EventosSemSala).


eventosSemSalasDiaSemana(DiaDaSemana, EventosSemSala) :-
    findall(E, (evento(E,_,_,_,semSala), horario(E,DiaDaSemana,_,_,_,_)), EventosSemSala).


eventosSemSalasPeriodo([], []).

eventosSemSalasPeriodo(ListaPeriodos, EventosSemSala) :-
    adicionaSemestres(ListaPeriodos, ListaSemestrada),
    findall(E, (evento(E,_,_,_,semSala), horario(E,_,_,_,_,P), member(P, ListaSemestrada)), EventosSemSala).


adicionaSemestres(ListaPeriodos, ListaSemestrada) :-
    (member(p1, ListaPeriodos) ; member(p2, ListaPeriodos)), append([p1_2], ListaPeriodos, ListaSemestrada), !;
    (member(p3, ListaPeriodos) ; member(p4, ListaPeriodos)), append([p3_4], ListaPeriodos, ListaSemestrada).


% Engorda o predicado.
organizaEventos(ListaEventos, Periodo, EventosNoPeriodo) :-
    organizaEventos(ListaEventos, Periodo, EventosNoPeriodo, []).

 % Caso base.
organizaEventos([], _, EventosNoPeriodo, EventosNoPeriodo).

% Caso recursivo, se o evento estiver no periodo, adiciona-o a lista de eventos no periodo.
organizaEventos([H|T], Periodo, EventosNoPeriodo, Aux) :-
    adicionaSemestres([Periodo], ListaSemestrada),
    horario(H, _, _, _, _, P),
    member(P, ListaSemestrada),
    append(Aux, [H], EventosNoPeriodoNovo),
    organizaEventos(T, Periodo, EventosNoPeriodo, EventosNoPeriodoNovo), !;
    organizaEventos(T, Periodo, EventosNoPeriodo, Aux).

