% ist1106336, Enzo Nunes Pedralho :)
:- set_prolog_flag(answer_write_options,[max_depth(0)]). % para listas completas
:- ['dados.pl'], ['keywords.pl']. % ficheiros a importar.

eventosSemSalas(EventosSemSalas) :- findall(ID, evento(ID, _, _, _, semSala), EventosSemSalas). % 