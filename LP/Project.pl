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
organizaEventos([], _, EventosNoPeriodoSorted, EventosNoPeriodo) :-
    sort(EventosNoPeriodo, EventosNoPeriodoSorted), !.

% Caso recursivo, se o evento estiver no periodo, adiciona-o a lista de eventos no periodo.
organizaEventos([H|T], Periodo, EventosNoPeriodo, Aux) :-
    adicionaSemestres([Periodo], ListaSemestrada),
    horario(H, _, _, _, _, P),
    member(P, ListaSemestrada),
    append([H], Aux, EventosNoPeriodoNovo),
    organizaEventos(T, Periodo, EventosNoPeriodo, EventosNoPeriodoNovo), !;
    organizaEventos(T, Periodo, EventosNoPeriodo, Aux).

% Predicado que permite obter a lista de ids dos eventos que tem duracao inferior a Duracao.
eventosMenoresQue(Duracao, ListaEventosMenoresSorted) :-
    findall(E, (horario(E,_,_,_,D,_), D =< Duracao), ListaEventosMenores),
    sort(ListaEventosMenores, ListaEventosMenoresSorted).

% Predicado que retorna True se o evento E tem duracao inferior a Duracao.
eventosMenoresQueBool(E, Duracao) :-
    horario(E,_,_,_,D,_),
    D =< Duracao.

% Predicado que permite obter a lista ordenada alfabeticamente de disciplinas de um dado curso.
procuraDisciplinas(Curso, ListaDisciplinasCursoSorted) :-
    findall(Disciplina, (evento(E,Disciplina,_,_,_), turno(E,Curso,_,_)), ListaDisciplinasCurso),
    sort(ListaDisciplinasCurso, ListaDisciplinasCursoSorted).

% Engorda Predicado.
organizaDisciplinas(ListaDisciplinas, Curso, [Semestre1, Semestre2]) :-
    organizaDisciplinas(ListaDisciplinas, Curso, [Semestre1, Semestre2], [[], []]).

% Caso base.
organizaDisciplinas([], _, [Semestre1Sorted, Semestre2Sorted], [Semestre1, Semestre2]) :-
    sort(Semestre1, Semestre1Sorted),
    sort(Semestre2, Semestre2Sorted), !.

% Caso recursivo, adiciona a disciplina a lista do semestre correspondente.
organizaDisciplinas([Disciplina|T], Curso, [Semestre1, Semestre2], [Semestre1Aux, Semestre2Aux]) :-
    procuraDisciplinas(Curso, ListaDisciplinasCurso),
    member(Disciplina, ListaDisciplinasCurso),
    evento(E, Disciplina,_,_,_), horario(E,_,_,_,_,P),
    (
    member(P, [p1, p2, p1_2]),
    append([Disciplina], Semestre1Aux, Semestre1Novo),
    organizaDisciplinas(T, Curso, [Semestre1, Semestre2], [Semestre1Novo, Semestre2Aux]), !;
    member(P, [p3, p4, p3_4]),
    append([Disciplina], Semestre2Aux, Semestre2Novo),
    organizaDisciplinas(T, Curso, [Semestre1, Semestre2], [Semestre1Aux, Semestre2Novo])
    ).