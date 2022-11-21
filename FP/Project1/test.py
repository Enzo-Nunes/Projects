def produto_interno(t1, t2):
    """ tuplo * tuplo -> real
        Determina o produto interno entre dois vetores sob a forma de tuplo."""

    prod = 0
    for i in range(len(t1)):
        prod += t1[i] * t2[i]
    return float(prod)

"""3.2.2"""
def verifica_convergencia(m, c, x, e):
    """ tuplo * tuplo * tuplo * real -> booleano
        Verifica se a estimativa "x" para a solução do sistema está próxima o suficiente do valor verdadeiro pela precisão "e"."""

    res = True
    for i in range(len(m)):
        p = produto_interno(m[i], x)
        if abs(p - c[i]) >= e:
            res = False
    return res

"""3.2.3"""
def retira_zeros_diagonal(m, c):
    """ tuplo * tuplo -> tuplo * tuplo
        Permuta as linhas da matriz "m" de modo a que não haja zeros na sua diagonal principal."""

    m = list(m)
    c = list(c)
    for i in range(len(m)):
        if  m[i][i] == 0:
            for j in range(len(m)):
                if  m[j][i] != 0 and m[i][j] != 0:
                    m[i], m[j] = m[j], m[i]
                    c[i], c[j] = c[j], c[i]
                    break
    return (tuple(m), tuple(c))

"""3.2.4"""
def eh_diagonal_dominante(m):
    """ tuplo -> booleano
        Verifica se a matriz "m" é diagonalmente dominante, ou seja, se, para cada linha,
        o valor absoluto da diagonal é superior à soma dos restantes valores absolutos da linha."""

    for i in m:
        sum = 0
        for x in i:
            sum += abs(x)
        diag = abs(m[m.index(i)][m.index(i)])
        if diag < sum - diag:
            return False
    return True
    
"""3.2.5"""
def resolve_sistema(m, c, e):
    """ tuplo * tuplo * real -> tuplo
        Determina a estimativa "x", de precisão "e", pelo método de Jacobi, para a solução
        do sistema de equações representado pela matriz "m" e o vetor de contantes "c"."""

    if not isinstance(m, tuple) or not isinstance(c, tuple) or not isinstance(e, float):
        raise ValueError("resolve_sistema: argumentos invalidos")
    if m == () or c == () or len(m) != len(c) or e <= 0:
        raise ValueError("resolve_sistema: argumentos invalidos")
    for x in m:
        if not isinstance(x, tuple):
            raise ValueError("resolve_sistema: argumentos invalidos")
        if len(x) != len(m[0]) or len(x) != len(m):
            raise ValueError("resolve_sistema: argumentos invalidos")
        for y in x:
            if not isinstance(y, (int, float)):
                raise ValueError("resolve_sistema: argumentos invalidos")
    for x in c:
        if not isinstance (x, (int, float)):
            raise ValueError("resolve_sistema: argumentos invalidos")    

    m_ord = (retira_zeros_diagonal(m, c))[0]
    c_ord = (retira_zeros_diagonal(m, c))[1]
    x = len(m_ord) * [0]
        
    if not eh_diagonal_dominante(m_ord):
        raise ValueError("resolve_sistema: matriz nao diagonal dominante")

    while not verifica_convergencia(m_ord, c_ord, x, e):
        x_old = x.copy()
        for i in range(len(m_ord)):
            x[i] =  x_old[i] + (c_ord[i] - produto_interno(m_ord[i], x_old))/m_ord[i][i]
    return tuple(x)

print(resolve_sistema(((2, -1, -1), (2, -9, 7), (-2, 5, -9)), (-8, 8, -6), 1e-20))