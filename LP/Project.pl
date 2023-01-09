% ist1106336, Enzo Nunes Pedralho :)
:- set_prolog_flag(answer_write_options,[max_depth(0)]). % para listas completas
:- ['dados.pl'], ['keywords.pl']. % ficheiros a importar.

eventosSemSalas(EventosSemSala) :-
    findall(E, evento(E,_,_,_,semSala), EventosSemSala).


eventosSemSalasDiaSemana(DiaDaSemana, EventosSemSala) :-
    findall(E, (
            evento(E,_,_,_,semSala),
            horario(E,DiaDaSemana,_,_,_,_)),
        EventosSemSala).


eventosSemSalasPeriodo([], []).

eventosSemSalasPeriodo(ListaPeriodos, EventosSemSala) :-
    adicionaSemestres(ListaPeriodos, ListaSemestrada),
    findall(E, (
            evento(E,_,_,_,semSala),
            horario(E,_,_,_,_,P),
            member(P, ListaSemestrada)),
        EventosSemSala).


adicionaSemestres(ListaPeriodos, ListaSemestrada) :-
    (member(p1, ListaPeriodos) ; member(p2, ListaPeriodos)),
        append([p1_2], ListaPeriodos, ListaSemestrada), !;
    (member(p3, ListaPeriodos) ; member(p4, ListaPeriodos)),
        append([p3_4], ListaPeriodos, ListaSemestrada).


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
    findall(Disciplina, (
            evento(E,Disciplina,_,_,_),
            turno(E,Curso,_,_)),
        ListaDisciplinasCurso),
    sort(ListaDisciplinasCurso, ListaDisciplinasCursoSorted).

% Engorda Predicado.
organizaDisciplinas(ListaDisciplinas, Curso, [Seme1, Seme2]) :-
    organizaDisciplinas(ListaDisciplinas, Curso, [Seme1, Seme2], [[], []]).

% Caso base.
organizaDisciplinas([], _, [Seme1Sorted, Seme2Sorted], [Seme1, Seme2]) :-
    sort(Seme1, Seme1Sorted),
    sort(Seme2, Seme2Sorted), !.

% Caso recursivo, adiciona a disciplina a lista do semestre correspondente.
organizaDisciplinas([Discip|T], Curso, [Seme1, Seme2], [Seme1Aux, Seme2Aux]) :-
    procuraDisciplinas(Curso, ListaDiscipCurso),
    member(Discip, ListaDiscipCurso),
    evento(E, Discip,_,_,_), horario(E,_,_,_,_,P),
    (
    member(P, [p1, p2, p1_2]),
    append([Discip], Seme1Aux, Seme1Novo),
    organizaDisciplinas(T, Curso, [Seme1, Seme2], [Seme1Novo, Seme2Aux]), !;
    member(P, [p3, p4, p3_4]),
    append([Discip], Seme2Aux, Seme2Novo),
    organizaDisciplinas(T, Curso, [Seme1, Seme2], [Seme1Aux, Seme2Novo])
    ).


horasCurso(Periodo, Curso, Ano, TotalHoras) :-
    adicionaSemestres([Periodo], ListaSemestrada),
    findall(E, turno(E, Curso, Ano, _), ListaEventos),
    sort(ListaEventos, ListaEventosSorted),
    findall(D, (
            horario(E, _, _, _, D, P),
            member(E, ListaEventosSorted),
            member(P, ListaSemestrada)),
        ListaHoras), !,
    sum_list(ListaHoras, TotalHoras).


evolucaoHorasCurso(Curso, Evolucao) :-
    findall((Ano, Periodo, NumHoras), (
            member(Ano, [1, 2, 3]),
            member(Periodo, [p1, p2, p1_2, p3, p4, p3_4]),
            horasCurso(Periodo, Curso, Ano, NumHoras)),
        Evolucao).


ocupaSlot(HoraInicioDada, HoraFimDada, HoraInicioEvento, HoraFimEvento, Horas) :-
    HoraInicioEvento >= HoraInicioDada, HoraFimEvento =< HoraFimDada,
    Horas is HoraFimEvento - HoraInicioEvento, !;

    HoraInicioEvento =< HoraInicioDada, HoraFimEvento >= HoraFimDada,
    Horas is HoraFimDada - HoraInicioDada, !;

    HoraInicioEvento >= HoraInicioDada, HoraFimEvento >= HoraFimDada,
    HoraInicioEvento =< HoraFimDada,
    Horas is HoraFimDada - HoraInicioEvento, !;

    HoraInicioEvento =< HoraInicioDada, HoraFimEvento =< HoraFimDada,
    HoraFimEvento >= HoraInicioDada,
    Horas is HoraFimEvento - HoraInicioDada.

numHorasOcupadas(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, SomaHoras) :-
    adicionaSemestres([Periodo], ListaSemestrada),
    salas(TipoSala, Salas),
    findall(Horas, (
            horario(E, DiaSemana, HoraInicioEvento, HoraFimEvento, _, P),
            evento(E, _, _, _, S),
            member(S, Salas), member(P, ListaSemestrada),
            ocupaSlot(HoraInicio, HoraFim, HoraInicioEvento, HoraFimEvento, Horas)),
        ListaHoras),
    sum_list(ListaHoras, SomaHoras).


ocupacaoMax(TipoSala, HoraInicio, HoraFim, Max) :-
    salas(TipoSala, Salas),
    length(Salas, NumSalas),
    Max is (HoraFim - HoraInicio) * NumSalas.


percentagem(SomaHoras, Max, Percentagem) :-
    Percentagem is (SomaHoras / Max) * 100.

    ocupacaoCritica(HoraInicio, HoraFim, Threshold, ResFinal) :-
        findall(casosCriticos(DiaSemana, TipoSala, PercentagemFinal), (
                member(Periodo, [p1, p2, p3, p4]),
                salas(TipoSala, ListaSalas),
                member(Sala, ListaSalas),
                evento(E, _, _, _, Sala),
                horario(E, DiaSemana, _, _, _, Periodo),
                numHorasOcupadas(Periodo, TipoSala, DiaSemana, HoraInicio, HoraFim, SomaHoras),
                ocupacaoMax(TipoSala, HoraInicio, HoraFim, Max),
                percentagem(SomaHoras, Max, Percentagem),
                Percentagem > Threshold,
                ceiling(Percentagem, PercentagemFinal)),
            Resultados),
        sort(Resultados, ResFinal).