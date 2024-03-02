% ist1106336, Enzo Nunes Pedralho :)
:- set_prolog_flag(answer_write_options,[max_depth(0)]). % para listas completas
:- ['dados.pl'], ['keywords.pl']. % ficheiros a importar.


% Predicado auxiliar que adiciona os semestres 1_2 e 3_4 a uma lista de periodos.
% Util em predicados que procurem por eventos, dado que ha tambem eventos semestrais.
adicionaSemestres(ListaPeriodos, ListaSemestrada) :-
    (member(p1, ListaPeriodos) ; member(p2, ListaPeriodos)),
        append([p1_2], ListaPeriodos, ListaSemestrada), !;
    (member(p3, ListaPeriodos) ; member(p4, ListaPeriodos)),
        append([p3_4], ListaPeriodos, ListaSemestrada).


% Predicado que permite obter a lista de ids dos eventos que nao tem sala.
eventosSemSalas(EventosSemSala) :-
    findall(E, evento(E,_,_,_,semSala), EventosSemSala).


% Predicado que permite obter a lista de ids dos eventos sem sala num dado dia da semana.
eventosSemSalasDiaSemana(DiaDaSemana, EventosSemSala) :-
    findall(E, (
            evento(E,_,_,_,semSala),
            horario(E,DiaDaSemana,_,_,_,_)),
        EventosSemSala).


% Predicado que permite obter a lista de ids dos eventos sem sala num dado periodo.
% Caso base
eventosSemSalasPeriodo([], []).


% Caso recursivo, se o eventos for do periodo correspondente, adiciona o evento
% a lista de eventos sem sala.
eventosSemSalasPeriodo(ListaPeriodos, EventosSemSala) :-
    adicionaSemestres(ListaPeriodos, ListaSemestrada),
    findall(E, (
            evento(E,_,_,_,semSala),
            horario(E,_,_,_,_,P),
            member(P, ListaSemestrada)),
        EventosSemSala).


% Predicado que permite obter uma lista de eventos num dado periodo.
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


% Predicado que retorna True se, e apenas se, o evento E tem duracao inferior a Duracao.
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


% Predicado que permite obter uma lista com duas listas ordenadas de eventos de
% um curso que ocorrem no primeiro e segundo semestre, respetivamente.
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
    evento(E, Discip,_,_,_), horario(E,_,_,_,_,P), (
    
    member(P, [p1, p2, p1_2]),
    append([Discip], Seme1Aux, Seme1Novo),
    organizaDisciplinas(T, Curso, [Seme1, Seme2], [Seme1Novo, Seme2Aux]), !;

    member(P, [p3, p4, p3_4]),
    append([Discip], Seme2Aux, Seme2Novo),
    organizaDisciplinas(T, Curso, [Seme1, Seme2], [Seme1Aux, Seme2Novo])).


% Predicado que permite obter o numero de horas dos eventos de um dado curso num dado periodo.
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


% Predicado que permite obter o numero de horas dos eventos organizados por Ano e Periodo.
evolucaoHorasCurso(Curso, Evolucao) :-
    findall((Ano, Periodo, NumHoras), (
            member(Ano, [1, 2, 3]),
            member(Periodo, [p1, p2, p1_2, p3, p4, p3_4]),
            horasCurso(Periodo, Curso, Ano, NumHoras)),
        Evolucao).


% Predicado que permite obter as horas ocupadas por um dado evento num dado slot.
ocupaSlot(HoraInicioSlot, HoraFimSlot, HoraInicioEvento, HoraFimEvento, Horas) :-
    HoraInicioEvento >= HoraInicioSlot, HoraFimEvento =< HoraFimSlot,
    Horas is HoraFimEvento - HoraInicioEvento, !;

    HoraInicioEvento =< HoraInicioSlot, HoraFimEvento >= HoraFimSlot,
    Horas is HoraFimSlot - HoraInicioSlot, !;

    HoraInicioEvento >= HoraInicioSlot, HoraFimEvento >= HoraFimSlot,
    HoraInicioEvento =< HoraFimSlot,
    Horas is HoraFimSlot - HoraInicioEvento, !;

    HoraInicioEvento =< HoraInicioSlot, HoraFimEvento =< HoraFimSlot,
    HoraFimEvento >= HoraInicioSlot,
    Horas is HoraFimEvento - HoraInicioSlot.


% Predicado que permite obter o total de horas ocupadas de uma sala num dado
% slot de um dia da semana, num dado periodo.
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


% Predicado que permite obter o maximo de horas que uma sala pode estar ocupada num dado slot.
ocupacaoMax(TipoSala, HoraInicio, HoraFim, Max) :-
    salas(TipoSala, Salas),
    length(Salas, NumSalas),
    Max is (HoraFim - HoraInicio) * NumSalas.


% Predicado que calcula percentagens. o_o
percentagem(SomaHoras, Max, Percentagem) :-
    Percentagem is (SomaHoras / Max) * 100.


% Predicado que permite obter a lista de eventos com percentagem de ocupacao superior
% a um dado threshold, organizados em tuplos de dia da semana, tipo de sala e percentagem.
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


% Predicado que permite organizar uma mesa retangular de 8 lugares de acordo com as restricoes dadas.
% Todas as possibilidades de mesas.
possibilidades(ListaPessoas, Possibilidades) :-
    findall([[X1, X2, X3], [X4, X5], [X6, X7, X8]], (
            permutation(ListaPessoas, [X1, X2, X3, X4, X5, X6, X7, X8])),
        Possibilidades).

% Engorda o predicado com todas as possibilidades de mesas.
ocupacaoMesa(ListaPessoas, ListaRestricoes, OcupacaoMesa) :-
    possibilidades(ListaPessoas, Possibilidades),
    ocupacaoMesa(ListaPessoas, ListaRestricoes, Possibilidades, OcupacaoMesa).

% Caso base.
ocupacaoMesa(_, [], Possibilidades, SolucaoFinal) :-
    append(Possibilidades, SolucaoFinal).

% Recursao principal para ver cada restricao, uma a uma.
ocupacaoMesa(ListaPessoas, [Restricao | Resto], Possibilidades, OcupacaoMesa) :-
    (Restricao = cab1(NomePessoa),
        findall(Solucao, (member(Solucao, Possibilidades), verificaCab1(Solucao, NomePessoa)), Mesas),
        ocupacaoMesa(ListaPessoas, Resto, Mesas, OcupacaoMesa)), !;

    (Restricao = cab2(NomePessoa),
        findall(Solucao, (member(Solucao, Possibilidades), verificaCab2(Solucao, NomePessoa)), Mesas),
        ocupacaoMesa(ListaPessoas, Resto, Mesas, OcupacaoMesa)), !;

    (Restricao = honra(NomePessoa1, NomePessoa2),
        findall(Solucao, (member(Solucao, Possibilidades), verificaHonra(Solucao, NomePessoa1, NomePessoa2)), Mesas),
        ocupacaoMesa(ListaPessoas, Resto, Mesas, OcupacaoMesa)), !;

    (Restricao = lado(NomePessoa1, NomePessoa2),
        findall(Solucao, (member(Solucao, Possibilidades), verificaLado(Solucao, NomePessoa1, NomePessoa2)), Mesas),
        ocupacaoMesa(ListaPessoas, Resto, Mesas, OcupacaoMesa)), !;

    (Restricao = naoLado(NomePessoa1, NomePessoa2),
        findall(Solucao, (member(Solucao, Possibilidades), verificaNaoLado(Solucao, NomePessoa1, NomePessoa2)), Mesas),
        ocupacaoMesa(ListaPessoas, Resto, Mesas, OcupacaoMesa)), !;

    (Restricao = frente(NomePessoa1, NomePessoa2),
        findall(Solucao, (member(Solucao, Possibilidades), verificaFrente(Solucao, NomePessoa1, NomePessoa2)), Mesas),
        ocupacaoMesa(ListaPessoas, Resto, Mesas, OcupacaoMesa)), !;

    (Restricao = naoFrente(NomePessoa1, NomePessoa2),
        findall(Solucao, (member(Solucao, Possibilidades), verificaNaoFrente(Solucao, NomePessoa1, NomePessoa2)), Mesas),
        ocupacaoMesa(ListaPessoas, Resto, Mesas, OcupacaoMesa)).

% Predicados auxiliares para verificar as restricoes.
verificaCab1(Solucao, NomePessoa) :-
    nth0(1, Solucao, [NomePessoa, _]).

verificaCab2(Solucao, NomePessoa) :-
    nth0(1, Solucao, [_, NomePessoa]).

verificaHonra(Solucao, NomePessoa1, NomePessoa2) :-
    (nth0(1, Solucao, [NomePessoa1, _]),
    nth0(2, Solucao, [NomePessoa2, _, _])), !;
    (nth0(1, Solucao, [_, NomePessoa1]),
    nth0(0, Solucao, [_, _, NomePessoa2])).

verificaLado(Solucao, NomePessoa1, NomePessoa2) :-
    nth0(0, Solucao, [NomePessoa1, NomePessoa2, _]), !;
    nth0(0, Solucao, [_, NomePessoa1, NomePessoa2]), !;
    nth0(2, Solucao, [NomePessoa1, NomePessoa2, _]), !;
    nth0(2, Solucao, [_, NomePessoa1, NomePessoa2]), !;
    nth0(0, Solucao, [NomePessoa2, NomePessoa1, _]), !;
    nth0(0, Solucao, [_, NomePessoa2, NomePessoa1]), !;
    nth0(2, Solucao, [NomePessoa2, NomePessoa1, _]), !;
    nth0(2, Solucao, [_, NomePessoa2, NomePessoa1]).

verificaNaoLado(Solucao, NomePessoa1, NomePessoa2) :-
    \+ verificaLado(Solucao, NomePessoa1, NomePessoa2).

verificaFrente(Solucao, NomePessoa1, NomePessoa2) :-
    (nth0(0, Solucao, [NomePessoa1, _, _]), 
        nth0(2, Solucao, [NomePessoa2, _, _])), !;
    (nth0(0, Solucao, [_, NomePessoa1, _]), 
        nth0(2, Solucao, [_, NomePessoa2, _])), !;
    (nth0(0, Solucao, [_, _, NomePessoa1]), 
        nth0(2, Solucao, [_, _, NomePessoa2])), !;
    (nth0(0, Solucao, [NomePessoa2, _, _]), 
        nth0(2, Solucao, [NomePessoa1, _, _])), !;
    (nth0(0, Solucao, [_, NomePessoa2, _]), 
        nth0(2, Solucao, [_, NomePessoa1, _])), !;
    (nth0(0, Solucao, [_, _, NomePessoa2]), 
        nth0(2, Solucao, [_, _, NomePessoa1])).

verificaNaoFrente(Solucao, NomePessoa1, NomePessoa2) :-
    \+ verificaFrente(Solucao, NomePessoa1, NomePessoa2).