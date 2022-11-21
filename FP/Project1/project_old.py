"""
    1.º Projeto em FP
    Enzo Nunes
    ist1106336
    pedralhoenzo@gmail.com
"""

"""1.2.1"""
def limpa_texto(t):
    """ string -> string
        Limpa o texto, t, de caracteres brancos, resultando numa string em que as palavras estão separadas por um espaço."""

    t = t.split()
    return " ".join(t)

"""1.2.2"""
def corta_texto(s, n):
    """ string * inteiro -> string * string
        Corta a entrada, s, segundo um dado tamanho de coluna, n. Retorna um tuplo com dois elementos que correspondem à entrada cortada."""

    if s == "":                                                     # Casos especiais.
        return ("", "")                                             #
    first_word = s.split()[0]                                       #
    if n < len(first_word):                                         #
        raise ValueError("justifica_texto: argumentos invalidos")   #
    elif n > len(s):                                                #
        return (s, "")                                              #
        
    else:
        space_before_n = s[n::-1].find(" ")                 # Corta a entrada no espaço anterior à posição "n" na entrada.
        cut = n - space_before_n                            #
        return (s[:cut], s[cut + 1:])                       #

"""1.2.3"""
def insere_espacos(s, n):
    """ string * inteiro -> string
        Insere espaços entre as palavras da entrada, s, até esta ter o tamanho de coluna desejado, n."""

    words = s.split()                                       # Casos especiais.
    if len(words) == 1:                                     #
        return s + (n - len(s)) * " "                       #
    if n < len(s):                                          #
        return s                                            #

    spaces = s.count(" ")                                   # Destribui os espaços necessários entre as palavras.
    length = len(s) - spaces                                #
    spaces_per_space = (n - length)//spaces                 #
    res = s.replace(" ", spaces_per_space * " ")            #
    
    if (n - length) % spaces == 0:                                                                  # Determina se devem ser adicionados espaços extra ou não para completar as linhas.
        return res                                                                                  #
    else:                                                                                           #
        spaces_extra = (n - length) % spaces                                                        #
        res = res.replace(spaces_per_space * " ", (spaces_per_space + 1) * " ", spaces_extra)       #
        
        return res                                                                                  

"""1.2.4"""
def justifica_texto(t, n):
    """ string * inteiro -> tuplo
        Justifica a entrada, t, de acordo com o tamanho de coluna desejado, n."""

    if not (isinstance(t, str) and isinstance(n, int) and n > 0 and t != ""):
        raise ValueError("justifica_texto: argumentos invalidos")
    clean = limpa_texto(t)
    if len(clean) < n:
        return (clean + (n - len(clean)) * " ", )

                                      
    clean = corta_texto(clean, n)                           # Divide a entrada em colunas com o tamanho desejado.
    cut = ()                                                #
    while len(clean[1]) > n:                                #
        cut = cut + (clean[0], )                            #
        clean = corta_texto(clean[1], n)                    #
    cut = cut + (clean[0], clean[1], )                      #

    res = ()                                                # Insere espaços em cada linha até estas terem o tamanho de coluna desejado.
    for i in range(len(cut[:-1])):                          #
        elem = insere_espacos(cut[i], n)                    #
        res = res + (elem, )                                #

    if cut[-1] == "":                                       # Adiciona espaços no fim da útima linha se necessário.
        return res                                          #
    res += (cut[-1] + (n - len((cut)[-1])) * " ", )         #
    return res                                              #


"""2.2.1"""
def calcula_quocientes(d, n):
    """ dicionário * inteiro -> dicionário
        Calcula os quocientes de cada partido indicado em d, concorrentes a um círculo com n deputados, segundo o Método de Hondt."""

    res = {}
    for i in d:
        l = []
        for j in range(1, n + 1):
            l += [d[i]/j]
        res[i] = l
    return res

"""2.2.2"""
def atribui_mandatos(d, n):
    """ dicionário * inteiro -> lista
        Retorna uma lista com os deputados apurados a cada mandato, n, por ordem, a partir dos votos de cada partido em d."""

    res             = []
    first_values    = list(d.values())
    values          = list(d.values())
    quocients       = calcula_quocientes(d, n)
    for i in range(n):                                                  
        max_value   = max(values)
        keys        = key_lookup_quocientes(quocients, max_value)       # Retorna as keys correspondentes a cada partido que possuam o valor máximo de votos.
        match       = len(keys.copy())
        while keys != [] and len(res) != n:                             # Adiciona os partidos ao resultado, por ordem.
            count   = [res.count(x) for x in keys]                      #
            res    += [keys[count.index(min(count))]]                   #
            keys.remove(keys[count.index(min(count))])                  #
        for j in range(match):                                                                              # Em "values", troca os valores adicionados ao resultado pelos seus próximos valores.
            values[values.index(max_value)] = next_value(max_value, first_values[values.index(max_value)])  #
    return res  

def key_lookup_quocientes(d, n):
    """ dicionário * inteiro -> lista
        Retorna uma lista com os partidos a que correspondem um dado quociente."""

    keys    = list(d.keys())
    values  = list(d.values())
    res     = []
    for i in range(len(values)):
        if n in values[i]:
            res += [keys[i]]
    return res

def next_value(v, first_v):
    """ inteiro * inteiro -> inteiro
        Retorna o quociente seguinte ao quociente "v" de um dado partido a partir do quociente inicial, "first_v"."""

    n = first_v/v
    return first_v/(n + 1)

"""2.2.3"""
def obtem_partidos(d):
    """ dicionário -> lista
        Retorna os partidos por ordem alfabética participantes nas eleições dos círculos de um dado território."""

    circulos    = list(d.values())
    votos       = [x["votos"] for x in circulos]
    partidos    = [list(x.keys()) for x in votos]
    res         = []
    for x in partidos:
        for y in x:
            if y not in res:
                res += [y]
    res.sort()
    return res

"""2.2.4"""
def obtem_resultado_eleicoes(d):
    """ dicionário -> lista
        Determina, pelo método de Hondt, o número de deputados eleitos de cada partido nos círculos de um dado território."""

    if not (isinstance(d, dict)):                                                               
        raise ValueError("obtem_resultado_eleicoes: argumento invalido")
    if len(d) == 0:
        raise ValueError("obtem_resultado_eleicoes: argumento invalido")
    for i in d:
        if not (isinstance(i, str) and isinstance(d[i], dict)):
            raise ValueError("obtem_resultado_eleicoes: argumento invalido")
        if not ("votos" in d[i] and "deputados" in d[i]):
            raise ValueError("obtem_resultado_eleicoes: argumento invalido")
        for j in d[i]:
            if not(j == "deputados" or j == "votos"):
                raise ValueError("obtem_resultado_eleicoes: argumento invalido")
        if not (isinstance(d[i]["deputados"], int) and isinstance(d[i]["votos"], dict)):
            raise ValueError("obtem_resultado_eleicoes: argumento invalido")
        if d[i]["votos"] == {} or d[i]["deputados"] <= 0:
            raise ValueError("obtem_resultado_eleicoes: argumento invalido")
        for k in d[i]["votos"]:
            if not (isinstance(k, str) and isinstance(d[i]["votos"][k], int)):
                raise ValueError("obtem_resultado_eleicoes: argumento invalido")
        for k in d[i]["votos"]:
            if d[i]["votos"][k] < 0:
                raise ValueError("obtem_resultado_eleicoes: argumento invalido")
        sum = 0
        for x in d[i]["votos"].values():
            sum += x
        if sum == 0:
            raise ValueError("obtem_resultado_eleicoes: argumento invalido")

    partidos    = obtem_partidos(d)
    circulos    = list(d.values())
    votos       = [x["votos"] for x in circulos]
    deputados   = [x["deputados"] for x in circulos]
    mandatos    = []
    for i in range(len(votos)):                                 # Cria a lista ordenada com o número de mandatos atribuidos a cada partido.
        mandatos += atribui_mandatos(votos[i], deputados[i])    #
    mandatos    = [mandatos.count(x) for x in partidos]         #
    
    res = []
    total_votos = [i * 0 for i in range(len(partidos))]    
    for i in range(len(partidos)):                              # Cria a lista com os resultados de cada partido nas eleições de um dado território.
        for x in votos:                                         #
            if partidos[i] in x.keys():                         #
                total_votos[i] += x[partidos[i]]                #
        res += [(partidos[i], mandatos[i], total_votos[i])]     # Ordena a lista de acordo com o número de deputados eleitos e votos obtidos.
    res.sort(key = lambda x: (x[1], x[2]), reverse = True)
    return res


"""3.2.1"""
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