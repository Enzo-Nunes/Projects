"""
    2.º Projeto em FP
    Enzo Nunes
    ist1106336
    enzo.nunes@tecnico.ulisboa.pt
"""

alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"


def carateres(c):
    """ str -> str
        Retorna  a  slice  do  alfabeto  que  vai até a entrada, c. Deste  modo,
        é  possível  realizar  certas  partes do projeto sem utilizar ASCII e as 
        funções built-in chr e ord."""
    return alfabeto[:alfabeto.index(c) + 1]


"""
    TAD gerador
    Representação  interna:  Lista  cujos  elementos são, por ordem, o número de
    bits do gerador e o seu estado atual ou seed, quando o gerador é criado.
"""


def cria_gerador(b, s):
    """ int * int -> gerador
        Cria o gerador representado pelo número de bits, b, e a seed, s."""
    if not (isinstance(b, int) and isinstance(s, int)):
        raise ValueError("cria_gerador: argumentos invalidos")
    if not ((b == 32 or b == 64) and s > 0):
        raise ValueError("cria_gerador: argumentos invalidos")
    if (b == 32 and s >= 0xFFFFFFFF) or (b == 64 and s >= 0xFFFFFFFFFFFFFFFF):
        raise ValueError("cria_gerador: argumentos invalidos")
    return [b, s]


def cria_copia_gerador(g):
    """ gerador -> gerador
        Retorna uma cópia do gerador g."""
    return g.copy()


def obtem_estado(g):
    """ gerador -> int
        Retorna o estado atual do gerador g."""
    return g[1]


def define_estado(g, s):
    """ gerador * int -> int
        Altera o estado do gerador g para s."""
    g[1] = s
    return s


def atualiza_estado(g):
    """ gerador -> int
        Atualiza o estado do gerador g segundo o algoritmo xorshift e devolve-o."""
    if g[0] == 32:
        g[1] ^= (g[1] << 13) & 0xFFFFFFFF
        g[1] ^= (g[1] >> 17) & 0xFFFFFFFF
        g[1] ^= (g[1] << 5) & 0xFFFFFFFF
    if g[0] == 64:
        g[1] ^= (g[1] << 13) & 0xFFFFFFFFFFFFFFFF
        g[1] ^= (g[1] >> 7) & 0xFFFFFFFFFFFFFFFF
        g[1] ^= (g[1] << 17) & 0xFFFFFFFFFFFFFFFF
    return g[1]


def eh_gerador(arg):
    """ universal -> bool
        Verifica se a entrada, arg, é um TAD gerador."""
    if not (isinstance(arg, list) and all(isinstance(x, int) for x in arg)):
        return False
    if len(arg) != 2:
        return False
    if not ((arg[0] == 32 or arg[0] == 64) and arg[1] > 0):
        return False
    if (arg[0] == 32 and arg[1] >= 0xFFFFFFFF) or (arg[0] == 64 and arg[1] >= 0xFFFFFFFFFFFFFFFF):
        return False
    return True


def geradores_iguais(g1, g2):
    """ gerador * gerador -> bool
        Verifica se as duas entradas, g1 e g2, são geradores e são iguais."""
    return eh_gerador(g1) and eh_gerador(g2) and g1 == g2


def gerador_para_str(g):
    """ gerador -> str
        Retorna  a  string  que  representa  o  gerador,  g,  de  acordo  com  a
        representação desejada."""
    b = g[0]
    s = g[1]
    return "xorshift{}(s={})".format(b, s)


def gera_numero_aleatorio(g, n):
    """ gerador * int -> int
        Retorna  um  número  aleatorio  entre  1  e  n  através  do  gerador  g,
        atualizando o seu estado."""
    return 1 + atualiza_estado(g) % n


def gera_carater_aleatorio(g, c):
    """ gerador * str -> str
        Retorna  um  carater  aleatorio  entre  A  e  c  através  do  gerador g,
        atualizando o seu estado."""
    return carateres(c)[atualiza_estado(g) % len(carateres(c))]


"""
    TAD coordenada
    Representação  interna:  Tuplo  cujos  elementos  são, por ordem, a coluna e
    linha que representam uma coordenada.
"""


def cria_coordenada(col, lin):
    """ str * int -> coordenada
        Cria a coordenada representada pela coluna, col, e linha, lin."""
    if not (isinstance(col, str) and isinstance(lin, int)):
        raise ValueError("cria_coordenada: argumentos invalidos")
    if not (len(col) == 1 and col in alfabeto):
        raise ValueError("cria_coordenada: argumentos invalidos")
    if not (1 <= lin <= 99):
        raise ValueError("cria_coordenada: argumentos invalidos")

    return col, lin


def obtem_coluna(c):
    """ coordenada -> str
        Retorna a coluna da coordenada c"""
    return c[0]


def obtem_linha(c):
    """ coordenada -> int
        Retorna a linha da coordenada c"""
    return c[1]


def eh_coordenada(arg):
    """ universal -> bool
        Verifica se a entrada, arg, é um TAD coordenada."""
    if not (isinstance(arg, tuple) and isinstance(arg[0], str) and isinstance(arg[1], int)):
        return False
    if not (len(arg[0]) == 1 and arg[0] in alfabeto):
        return False
    if not (1 <= arg[1] <= 99):
        return False
    return True


def coordenadas_iguais(c1, c2):
    """ coordenada * coordenada -> bool
        Verifica se as duas entradas, c1 e c2, são coordenadas e são iguais."""
    return eh_coordenada(c1) and eh_coordenada(c2) and obtem_coluna(c1) == obtem_coluna(c2) and obtem_linha(c1) == obtem_linha(c2)


def coordenada_para_str(c):
    """ coordenada -> str
        Retorna  a  string  que  representa  a  coordenada,  c,  de acordo com a
        representação desejada."""
    return obtem_coluna(c) + ("0" if obtem_linha(c) < 10 else "") + str(obtem_linha(c))


def str_para_coordenada(s):
    """ str -> coordenada
        Retorna a coordenada representada pela str s"""
    try:
        return (s[0], int(s[1:]))
    except:
        pass


def obtem_coordenadas_vizinhas(c):
    """ coordenada -> tuplo
        Retorna  um  tuplo  cujos  elementos  são  as  coordenadas vizinhas a c,
        por ordem, no sentido horário, começando na superior esquerda."""
    # Variáveis possíveis das coordenadas em volta de c.
    col_ant = alfabeto[alfabeto.index(obtem_coluna(
        c)) - 1] if obtem_coluna(c) != "A" else ""
    col_seg = alfabeto[alfabeto.index(obtem_coluna(
        c)) + 1] if obtem_coluna(c) != "Z" else ""
    lin_ant = obtem_linha(c) - 1
    lin_seg = obtem_linha(c) + 1

    # Define todas as posições possíveis em volta de c.
    cima_esq = cria_coordenada(
        col_ant, lin_ant) if col_ant != "" and lin_ant != 0 else ()
    cima = cria_coordenada(obtem_coluna(c), lin_ant) if lin_ant != 0 else ()
    cima_dir = cria_coordenada(
        col_seg, lin_ant) if col_seg != "" and lin_ant != 0 else ()
    direita = cria_coordenada(col_seg, obtem_linha(c)) if col_seg != "" else ()
    baixo_dir = cria_coordenada(
        col_seg, lin_seg) if col_seg != "" and lin_seg != 100 else ()
    baixo = cria_coordenada(obtem_coluna(c), lin_seg) if lin_seg != 100 else ()
    baixo_esq = cria_coordenada(
        col_ant, lin_seg) if col_ant != "" and lin_seg != 100 else ()
    esquerda = cria_coordenada(
        col_ant, obtem_linha(c)) if col_ant != "" else ()

    return tuple(x for x in (cima_esq, cima, cima_dir, direita, baixo_dir, baixo, baixo_esq, esquerda) if x != ())


def obtem_coordenada_aleatoria(c, g):
    """ coordenada -> tuplo
        Retorna uma coordenada aleatória a partir da coordenada c e do gerador g."""
    return cria_coordenada(gera_carater_aleatorio(g, obtem_coluna(c)), gera_numero_aleatorio(g, obtem_linha(c)))


"""
    TAD parcela
    Representação  interna:  Dicionário  de  keys  "estado"  e "tem_mina", cujos
    valores são, respetivamente, "tapada", "marcada" ou "limpa" e True ou False.
"""


def cria_parcela():
    """ {} -> parcela
        Retorna uma parcela tapda sem mina."""
    return {"estado": "tapada", "tem_mina": False}


def cria_copia_parcela(p):
    """ parcela -> parcela
        Retorna uma cópia da parcela p."""
    return p.copy()


def limpa_parcela(p):
    """ parcela -> parcela
        Limpa e retorna a parcela p."""
    p["estado"] = "limpa"
    return p


def marca_parcela(p):
    """ parcela -> parcela
        Marca e retorna a parcela p."""
    p["estado"] = "marcada"
    return p


def desmarca_parcela(p):
    """ parcela -> parcela
        Desmarca e retorna a parcela p."""
    p["estado"] = "tapada"
    return p


def esconde_mina(p):
    """ parcela -> parcela
        Esconde uma mina e retorna a parcela p."""
    p["tem_mina"] = True
    return p


def eh_parcela(arg):
    """ universal -> bool
        Verifica se a entrada, arg, é um TAD parcela."""
    if not (isinstance(arg, dict) and len(arg) == 2):
        return False
    for x in arg:
        if not (x == "estado" or x == "tem_mina"):
            return False
    if not (arg["estado"] == "tapada" or arg["estado"] == "limpa" or arg["estado"] == "marcada"):
        return False
    if not isinstance(arg["tem_mina"], bool):
        return False
    return True


def eh_parcela_tapada(p):
    """ parcela -> bool
        Verifica se a parcela, p, está tapada."""
    return p["estado"] == "tapada"


def eh_parcela_marcada(p):
    """ parcela -> bool
        Verifica se a parcela, p, está marcada."""
    return p["estado"] == "marcada"


def eh_parcela_limpa(p):
    """ parcela -> bool
        Verifica se a parcela, p, está limpa."""
    return p["estado"] == "limpa"


def eh_parcela_minada(p):
    """ parcela -> bool
        Verifica se a parcela, p, está minada."""
    return p["tem_mina"]


def parcelas_iguais(p1, p2):
    """ parcela * parcela -> bool
        Verifica se as duas entradas, p1 e p2, são parcelas e são iguais."""
    return eh_parcela(p1) and eh_parcela(p2) and p1 == p2


def parcela_para_str(p):
    """ parcela -> str
        Retorna  a  string  que  representa  a  parcela,  p,  de   acordo  com a
        representação desejada."""
    if eh_parcela_tapada(p):
        return "#"
    if eh_parcela_marcada(p):
        return "@"
    if eh_parcela_minada(p):
        return "X"
    return "?"


def alterna_bandeira(p):
    """ parcela -> bool
        Desmarca  ou  marca  se  a  parcela  p, respetivamente, tiver ou não uma
        bandeira, retornando True. Retorna False em qualquer outro caso."""
    if eh_parcela_marcada(p):
        desmarca_parcela(p)
        return True
    if eh_parcela_tapada(p):
        marca_parcela(p)
        return True
    return False


"""
    TAD campo
    Representação interna: Matriz, ou seja, lista cujos elementos são listas que
    representam  linhas  cujos elementos são TADs parcela. O número de linhas do
    campo  corresponde  ao  comprimento  do  campo  e  o  número  de  colunas ao
    comprimento de qualquer elemento do campo.
"""


def cria_campo(c, l):
    """ str * int -> campo
        Cria o campo com l linhas e colunas de A até c."""
    if not (isinstance(c, str) and isinstance(l, int)):
        raise ValueError("cria_campo: argumentos invalidos")
    if not (len(c) == 1 and c in alfabeto):
        raise ValueError("cria_campo: argumentos invalidos")
    if not (1 <= l <= 99):
        raise ValueError("cria_campo: argumentos invalidos")

    campo = []
    for i in range(l):
        campo += [[cria_parcela() for j in range(len(carateres(c)))]]
    return campo


def cria_copia_campo(m):
    """ campo -> campo
        Retorna uma cópia do campo m."""
    copia = []
    for i in range(len(m)):
        linha = []
        for j in range(len(m[i])):
            parcela = {}
            for k in m[i][j]:
                parcela[k] = m[i][j][k]
            linha += [parcela]
        copia += [linha]
    return copia


def obtem_ultima_coluna(m):
    """ campo -> str
        Retorna a última coluna do campo m"""
    return alfabeto[len(m[0]) - 1]


def obtem_ultima_linha(m):
    """ campo -> int
        Retorna a última linha do campo m"""
    return len(m)


def obtem_parcela(m, c):
    """ campo * coordenada -> parcela
        Retorna a parcela do campo m na posição representada pela coordenada c."""
    return m[obtem_linha(c) - 1][alfabeto.index(obtem_coluna(c))]


def obtem_coordenadas(m, s):
    """ campo * str -> tuplo
        Retorna  o  tuplo  formado  pelas  parcelas ordenadas da esquerda para a
        direita  e  de  cima  pra  baixo  de  acordo com s, "tapadas", "limpas",
        "marcadas" ou "minadas"."""
    if s == "tapadas":
        return tuple(cria_coordenada(alfabeto[j], i + 1) for i in range(len(m)) for j in range(len(m[i])) if eh_parcela_tapada(m[i][j]))
    if s == "limpas":
        return tuple(cria_coordenada(alfabeto[j], i + 1) for i in range(len(m)) for j in range(len(m[i])) if eh_parcela_limpa(m[i][j]))
    if s == "marcadas":
        return tuple(cria_coordenada(alfabeto[j], i + 1) for i in range(len(m)) for j in range(len(m[i])) if eh_parcela_marcada(m[i][j]))
    return tuple(cria_coordenada(alfabeto[j], i + 1) for i in range(len(m)) for j in range(len(m[i])) if eh_parcela_minada(m[i][j]))


def obtem_numero_minas_vizinhas(m, c):
    """ campo * coordenada -> int
        Retorna  o  número  de  minas vizinhas à parcela na posição representada
        pela coordenada c no campo m."""
    return len([x for x in obtem_coordenadas_vizinhas(c) if eh_coordenada_do_campo(m, x) and eh_parcela_minada(obtem_parcela(m, x))])


def eh_campo(arg):
    """ universal -> bool
        Verifica se a entrada, arg, é um TAD campo."""
    if not isinstance(arg, list):
        return False
    if len(arg) == 0:
        return False
    for i in range(len(arg)):
        if not isinstance(arg[i], list):
            return False
        for j in range(len(arg)):
            if len(arg[i]) != len(arg[j]):
                return False
        for x in arg[i]:
            if not eh_parcela(x):
                return False
    return True


def eh_coordenada_do_campo(m, c):
    """ campo * coordenada -> bool
        Verifica se a coordenada c pertence ao campo m."""
    if not eh_coordenada(c):
        return False
    if obtem_coluna(c) not in carateres(obtem_ultima_coluna(m)):
        return False
    if obtem_linha(c) not in range(1, obtem_ultima_linha(m) + 1):
        return False
    return True


def campos_iguais(m1, m2):
    """ campo * campo -> bool
        Verifica se as duas entradas, m1 e m2, são campos e são iguais."""
    if not eh_campo(m1) or not eh_campo(m2):
        return False
    if len(m1) != len(m2):
        return False
    for i in range(len(m1)):
        if len(m1[i]) != len(m2[i]):
            return False
        for j in range(len(m1[i])):
            if m1[i][j] != m2[i][j]:
                return False
    return True


def campo_para_str(m):
    """ campo -> str
        Retorna  a string que representa o campo m de acordo com a representação
        desejada."""
    campo = "   " + alfabeto[:(len(m[0]))] + "\n"\
        "  " + "+" + len(m[0]) * "-" + "+" + "\n"
    for i in range(len(m)):
        linha = ""
        for j in range(len(m[i])):
            if parcela_para_str(m[i][j]) == "?":
                p = str(obtem_numero_minas_vizinhas(
                    m, cria_coordenada(alfabeto[j], i + 1)))
                if p == "0":
                    p = " "
            else:
                p = parcela_para_str(m[i][j])
            linha += p
        campo += (("0" + str(i + 1)) if i < 9 else str(i + 1)) + \
            "|" + linha + "|" + "\n"
    return campo + "  " + "+" + len(m[0]) * "-" + "+"


def coloca_minas(m, c, g, n):
    """ campo * coordenada * gerador * int -> campo
        Esconde n minas pelo campo m em parcelas aleatórias excluindo na parcela
        da coordenada c e as suas vizinhas."""
    nao_minaveis = (c, ) + obtem_coordenadas_vizinhas(c)
    # Esconde minas aleatoriamente excluindo em nao_minaveis.
    while len(obtem_coordenadas(m, "minadas")) < n:
        coord = obtem_coordenada_aleatoria(cria_coordenada(
            obtem_ultima_coluna(m), obtem_ultima_linha(m)), g)
        if coord not in nao_minaveis:
            esconde_mina(obtem_parcela(m, coord))
            nao_minaveis += (coord, )
    return m


def limpa_campo(m, c):
    """ campo * coordenada -> campo
        Limpa  o campo m na coordenada c e recursivamente limpa as suas parcelas
        vizinhas caso estas não tenham minas vizinhas."""
    if eh_parcela_limpa(obtem_parcela(m, c)):
        return m
    limpa_parcela(obtem_parcela(m, c))
    if eh_parcela_minada(obtem_parcela(m, c)):
        return m
    if obtem_numero_minas_vizinhas(m, c) == 0:
        for x in obtem_coordenadas_vizinhas(c):
            if eh_coordenada_do_campo(m, x) and eh_parcela_tapada(obtem_parcela(m, x)):
                if obtem_numero_minas_vizinhas(m, x) != 0:
                    limpa_parcela(obtem_parcela(m, x))
                else:
                    limpa_campo(m, x)

    return m


def jogo_ganho(m):
    """ campo -> bool
        Verifica se o campo m representa um jogo de Minas vencido."""
    return len(obtem_coordenadas(m, "limpas")) + len(obtem_coordenadas(m, "minadas")) == obtem_ultima_linha(m) * len(carateres(obtem_ultima_coluna(m)))


def turno_jogador(m):
    """ campo -> bool
        Permite  ao  utilizador  reaizar  uma jogada sobre o campo m, retornando
        False caso este limpe uma parcela minada e True em qualquer outro caso."""
    jogada = str(input("Escolha uma ação, [L]impar ou [M]arcar:"))
    while jogada != "L" and jogada != "M":
        jogada = str(input("Escolha uma ação, [L]impar ou [M]arcar:"))
    coord = str_para_coordenada(input('Escolha uma coordenada:'))
    while not eh_coordenada_do_campo(m, coord):
        coord = str_para_coordenada(input('Escolha uma coordenada:'))

    if jogada == "L":
        limpa_campo(m, coord)
        if eh_parcela_minada(obtem_parcela(m, coord)):
            return False
        return True
    alterna_bandeira(obtem_parcela(m, coord))
    return True


def minas(c, l, n, d, s):
    """ str * int * int * int * int
        Função  principal que permite jogar Minas. Utiliza um campo com n minas,
        de dimensões c e l e um gerador de "d" bits e seed s."""
    # Tenta criar os TADs.
    try:
        cria_coordenada(c, l)
        cria_gerador(d, s)
    except:
        raise ValueError("minas: argumentos invalidos")
    if not (isinstance(n, int) and (0 < n <= (len(carateres(c)) * l) - 10)):
        raise ValueError("minas: argumentos invalidos")

    m = cria_campo(c, l)
    g = cria_gerador(d, s)
    b = len(obtem_coordenadas(m, "marcadas"))
    bandeiras = "   [Bandeiras {}/{}]".format(b, n)

    # Primeira jogada.
    print(bandeiras)
    print(campo_para_str(m))
    coord = str_para_coordenada(input('Escolha uma coordenada:'))
    while not eh_coordenada_do_campo(m, coord):
        coord = str_para_coordenada(input('Escolha uma coordenada:'))
    coloca_minas(m, coord, g, n)
    limpa_campo(m, coord)

    # Repete jogadas até o jogo ser vencido ou perdido.
    while not jogo_ganho(m):
        b = len(obtem_coordenadas(m, "marcadas"))
        bandeiras = "   [Bandeiras {}/{}]".format(b, n)
        print(bandeiras)
        print(campo_para_str(m))
        if not turno_jogador(m):
            b = len(obtem_coordenadas(m, "marcadas"))
            bandeiras = "   [Bandeiras {}/{}]".format(b, n)
            print(bandeiras)
            print(campo_para_str(m))
            print("BOOOOOOOM!!!")
            return False
    print(bandeiras)
    print(campo_para_str(m))
    print("VITORIA!!!")
    return True
