import pytest
from Project1 import *

class TestPublicJustificarTextos:

    def test_1(self):
        assert 'Fundamentos da Programacao' == limpa_texto('  Fundamentos\n\tda      Programacao\n')
    
    def test_2(self):
        assert ('Fundamentos da', 'Programacao') == corta_texto('Fundamentos da Programacao', 15)

    def test_3(self):
        assert 'Fundamentos  da Programacao!!!' == insere_espacos('Fundamentos da Programacao!!!', 30)

    def test_4(self):
        assert 'Fundamentos       da      Programacao!!!' == insere_espacos('Fundamentos da Programacao!!!', 40)

    def test_5(self):

        cad = ('Computers are incredibly  \n\tfast,     \n\t\taccurate'
            ' \n\t\t\tand  stupid.   \n    Human beings are incredibly  slow  '
            'inaccurate, and brilliant. \n     Together  they  are powerful   ' 
            'beyond imagination.')

        ref = ('Computers  are  incredibly  fast, accurate and stupid. Human',
            'beings   are  incredibly  slow  inaccurate,  and  brilliant.',
            'Together they are powerful beyond imagination.              ')
        assert ref == justifica_texto(cad, 60)

  

class TestPublicMetodoHondt:

    def test_1(self):

        ref =  {'A': [12000.0, 6000.0, 4000.0, 3000.0, 2400.0, 2000.0, 12000/7],
                    'B': [7500.0, 3750.0, 2500.0, 1875.0, 1500.0, 1250.0, 7500/7],
                    'C': [5250.0, 2625.0, 1750.0, 1312.5, 1050.0, 875.0, 750.0],
                    'D': [3000.0, 1500.0, 1000.0, 750.0, 600.0, 500.0, 3000/7]}

        hyp = calcula_quocientes({'A': 12000, 'B': 7500, 'C': 5250, 'D': 3000}, 7)
        
        assert ref == hyp

    def test_2(self):
        ref = ['A', 'B', 'A', 'C', 'A', 'B', 'D']
        assert ref == atribui_mandatos({'A': 12000, 'B': 7500, 'C': 5250, 'D': 3000}, 7)

    def test_3(self):
        info = {
            'Endor':   {'deputados': 7, 
                        'votos': {'A':12000, 'B':7500, 'C':5250, 'D':3000}},
            'Hoth':    {'deputados': 6, 
                        'votos': {'A':9000, 'B':11500, 'D':1500, 'E':5000}},
            'Tatooine': {'deputados': 3, 
                        'votos': {'A':3000, 'B':1900}}}

        ref = ['A', 'B', 'C', 'D', 'E']
        
        assert ref == obtem_partidos(info)

    def test_4(self):
        info = {
            'Endor':   {'deputados': 7, 
                        'votos': {'A':12000, 'B':7500, 'C':5250, 'D':3000}},
            'Hoth':    {'deputados': 6, 
                        'votos': {'A':9000, 'B':11500, 'D':1500, 'E':5000}},
            'Tatooine': {'deputados': 3, 
                        'votos': {'A':3000, 'B':1900}}}
        ref = [('A', 7, 24000), ('B', 6, 20900), ('C', 1, 5250), ('E', 1, 5000), ('D', 1, 4500)]
        
        assert ref == obtem_resultado_eleicoes(info)


class TestPublicSistemasLineares:

    def test_1(self):
        assert produto_interno((1,2,3,4,5),(-4,5,-6,7,-8)) == -24.0

    def test_2(self):
        assert verifica_convergencia(((1, -0.5), (-1, 2)), (-0.4, 1.9), (0.1001, 1), 0.00001) == False

    def test_3(self):
        assert verifica_convergencia(((1, -0.5), (-1, 2)), (-0.4, 1.9), (0.1001, 1), 0.001) == True

    def test_4(self):
        assert retira_zeros_diagonal(((0, 1, 1), (1, 0, 0), (0, 1, 0)), (1, 2, 3)) == (((1, 0, 0), (0, 1, 0), (0, 1, 1)), (2, 3, 1))

    def test_5(self):
        assert eh_diagonal_dominante(((1, 2, 3, 4, 5),(4, -5, 6, -7, 8), (1, 3, 5, 3, 1), (-1, 0, -1, 0, -1), (0, 2, 4, 6, 8))) == False

    def test_6(self):
        assert eh_diagonal_dominante(((1, 0, 0), (0, 1, 0), (0, 1, 1))) == True

    def test_7(self):    
        def equal(x,y):
            delta = 1e-10
            return all(abs(x[i]-y[i])<delta for i in range(len(x)))

        A4, c4 = ((2, -1, -1), (2, -9, 7), (-2, 5, -9)), (-8, 8, -6)
        ref = (-4.0, -1.0, 1.0)
            
        assert equal(resolve_sistema(A4, c4, 1e-20), ref)
