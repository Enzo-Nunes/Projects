organizaDisciplinas([], _, [Sem1Sorted, Sem2Sorted], [Sem1, Sem2]) :-
    sort(Sem1, Sem1Sorted),
    sort(Sem2, Sem2Sorted), !.