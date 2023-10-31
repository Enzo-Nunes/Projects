def caracter(str:str, n:int) -> str:
	""" Recebe um caracter e um inteiro e devolve o caracter deslocado n posições."""

	return chr(ord(str) + n)

def indice(str:str) -> int:
	""" Recebe um caracter e devolve o seu índice no alfabeto."""

	return ord(str) - ord("A")

"""TAD Interseção."""
def cria_intersecao(col:str, lin:int) -> 'tuple[str,int]':
	"""	Recebe dois inteiros e cria um interseção. Verifica a validade dos parâmetros."""
	if not (isinstance(lin, int) and isinstance(col, str)):
		raise ValueError("cria_intersecao: argumentos invalidos")
	if not ("A" <= col <= "S") or len(col) != 1:
		raise ValueError("cria_intersecao: argumentos invalidos")
	if not 1 <= lin <= 19:
		raise ValueError("cria_intersecao: argumentos invalidos") 

	return (col, lin)


def obtem_col(i:'tuple[str,int]') -> str:
	"""	Recebe uma interseção e devolve a coluna."""

	return i[0]


def obtem_lin(i:'tuple[str,int]') -> int:
	"""	Recebe uma interseção e devolve a linha."""

	return i[1]


def eh_intersecao(arg:any) -> bool:
	"""	Recebe um argumento e verifica se é uma interseção."""

	if not isinstance(arg, tuple):
		return False
	if not len(arg) == 2:
		return False
	if not isinstance(arg[0], str) or not isinstance(arg[1], int):
		return False
	if not ("A" <= arg[0] <= "S"):
		return False
	if not 1 <= arg[1] <= 19:
		return False
	
	return True


def intersecoes_iguais(i1:'tuple[str,int]', i2:'tuple[str,int]') -> bool:
	"""	Recebe duas interseções e verifica se são iguais."""

	if not (eh_intersecao(i1) and eh_intersecao(i2)):
		return False
	if not obtem_col(i1) == obtem_col(i2):
		return False
	if not obtem_lin(i1) == obtem_lin(i2):
		return False
	
	return True


def intersecao_para_str(i:'tuple[str,int]') -> str:
	"""	Recebe uma interseção e devolve uma string."""

	return str(obtem_col(i)) + str(obtem_lin(i))


def str_para_intersecao(s:str) -> 'tuple[str,int]':
	"""	Recebe uma string e devolve uma interseção."""

	return cria_intersecao(s[0], int(s[1:]))


def eh_str_intersecao(s:str) -> bool:
	"""	Recebe uma string e verifica se é a representação externa de uma interseção."""

	if not isinstance(s, str):
		return False
	if len(s) not in (2, 3):
		return False
	if not ("A" <= s[0] <= "S") and len(s[0]) != 1:
		return False
	if not s[1:].isdigit():
		return False
	
	return True


def obtem_intersecao_deslocada(i:'tuple[str,int]', n_cols:int, n_lins:int) -> 'tuple[str,int]':
	"""	Recebe uma interseção e dois inteiros que representam um deslocamento em
		colunas e linhas e devolve a interseção deslocada."""

	return cria_intersecao(caracter(obtem_col(i), n_cols), obtem_lin(i) + n_lins)


def obtem_intersecoes_adjacentes(i:'tuple[str,int]', l:'tuple[str,int]') -> 'tuple[tuple[str,int]]':
	"""	Recebe uma interseção e a interseção superior direita do campo e devolve
		um tuplo com as interseções adjacentes."""

	# Interseção
	col = obtem_col(i)
	lin = obtem_lin(i)

	# Limites do Campo
	lower = 1
	left  = "A"
	right = obtem_col(l)
	upper = obtem_lin(l)

	# Posições adjacentes
	esquerda = obtem_intersecao_deslocada(i, -1, 0) if col != left	else ()
	direita	 = obtem_intersecao_deslocada(i, +1, 0) if col != right else ()
	baixo	 = obtem_intersecao_deslocada(i, 0, -1) if lin != lower else ()
	cima	 = obtem_intersecao_deslocada(i, 0, +1) if lin != upper else ()

	return tuple(adjacente for adjacente in (baixo, esquerda, direita, cima) if adjacente != ())


def ordena_intersecoes(t:'tuple[str,int]') -> 'tuple[str,int]':
	"""	Recebe um tuplo de interseções e devolve o mesmo tuplo ordenado."""

	return tuple(sorted(t, key=lambda x: (obtem_lin(x), obtem_col(x))))


"""TAD Pedra."""
def cria_pedra_branca() -> str:
	"""	Cria uma pedra branca."""
	
	return "O"


def cria_pedra_preta() -> str:
	"""	Cria uma pedra preta."""

	return "X"


def cria_pedra_neutra() -> str:
	"""	Cria uma pedra."""

	return "."


def eh_pedra(arg:any) -> bool:
	"""	Recebe um argumento de qualquer tipo e erifica se é uma pedra."""

	if not isinstance(arg, str):
		return False
	if arg not in ("O", "X", "."):
		return False
	
	return True


def eh_pedra_branca(p:str) -> bool:
	"""	Recebe uma pedra e verifica se é branca."""

	if not eh_pedra(p):
		return False

	return p == "O"


def eh_pedra_preta(p:str) -> bool:
	"""	Recebe uma pedra e verifica se é preta."""

	if not eh_pedra(p):
		return False

	return p == "X"


def pedras_iguais(p1:str, p2:str) -> bool:
	"""	Recebe duas pedras e verifica se são iguais."""

	return p1 == p2


def pedra_para_str(p:str) -> str:
	"""	Recebe uma pedra e devolve uma string."""

	return p


def eh_pedra_jogador(p:str) -> bool:
	"""	Recebe uma pedra e verifica se é de um jogador."""

	if not eh_pedra(p):
		return False
	
	return eh_pedra_branca(p) or eh_pedra_preta(p)


"""TAD Goban."""
def cria_goban_vazio(n:int) -> 'list[list[str]]':
	"""	Cria um goban vazio."""

	if not isinstance(n, int):
		raise ValueError("cria_goban_vazio: argumentos invalidos")
	if n not in (9, 13, 19):
		raise ValueError("cria_goban_vazio: argumentos invalidos")


	return [[cria_pedra_neutra() for col in range(n)] for lin in range(n)]


def cria_goban(n:int, ib:'tuple[tuple[str,int]]', ip:'tuple[tuple[str,int]]') -> 'list[list[str]]':
	""" Cria um goban de tamanho n x n, com as interseçõe do tuplo ib ocupadas por
		pedras brancas e as interseções do tuplo ip ocupadas por pedras pretas."""
	
	if not isinstance(n, int):
		raise ValueError("cria_goban: argumentos invalidos")
	if not isinstance(ib, tuple) or not isinstance(ip, tuple):
		raise ValueError("cria_goban: argumentos invalidos")
	if n not in (9, 13, 19):
		raise ValueError("cria_goban: argumentos invalidos")
	
	g = cria_goban_vazio(n)

	for interseção in ib:
		if not eh_intersecao_valida(g, interseção) or interseção in ip or not ib.count(interseção):
			raise ValueError("cria_goban: argumentos invalidos")
		coloca_pedra(g, interseção, cria_pedra_branca())

	for interseção in ip:
		if not eh_intersecao_valida(g, interseção) or interseção in ib or not ip.count(interseção):
			raise ValueError("cria_goban: argumentos invalidos")
		coloca_pedra(g, interseção, cria_pedra_preta())

	return g


def cria_copia_goban(g:'list[list[str]]') -> 'list[list[str]]':
	"""	Recebe um goban e devolve uma cópia."""

	return [list(col) for col in g]


def obtem_ultima_intersecao(g:'list[list[str]]') -> 'tuple[str,int]':
	"""	Recebe um goban e devolve a sua última interseção."""

	return cria_intersecao(caracter("A", len(g) - 1), len(g))


def obtem_pedra(g:'list[list[str]]', i:'tuple[str,int]') -> str:
	"""	Recebe um goban e uma interseção e devolve a pedra nessa interseção."""

	return g[indice(obtem_col(i))][obtem_lin(i) - 1]


def obtem_cadeia(g:'list[list[str]]', i:'tuple[str,int]') -> 'tuple[tuple[str,int]]':
	""" Recebe um goban e uma intersecao e devolve um tuplo ordenado com
		todas as intersecoes connectadas a i."""
	
	pedra_i = obtem_pedra(g, i)
	last	= obtem_ultima_intersecao(g)

	# Intersecoes a verificar
	stack	= [i]
	# Intersecoes verificadas (resultado)
	res		= []

	# Algoritmo de busca em profundidade.
	while len(stack) > 0:
		current = stack.pop()
		if current not in res:
			res.append(current)
			for adjacente in obtem_intersecoes_adjacentes(current, last):
				if  pedras_iguais(pedra_i, obtem_pedra(g, adjacente)) and \
					adjacente not in res and adjacente not in stack:
					stack.append(adjacente)

	return ordena_intersecoes(tuple(res))


def coloca_pedra(g:'list[list[str]]', i:'tuple[str,int]', p:str) -> list:
	"""	Recebe um goban, uma interseção e uma pedra e coloca a pedra na interseção."""

	g[indice(obtem_col(i))][obtem_lin(i) - 1] = p

	return g


def remove_pedra(g:'list[list[str]]', i:'tuple[str,int]') -> 'list[list[str]]':
	"""	Recebe um goban e uma interseção e remove a pedra da interseção."""

	coloca_pedra(g, i, cria_pedra_neutra())

	return g


def remove_cadeia(g:'list[list[str]]', t:'tuple[tuple[str,int]]') -> 'list[list[str]]':
	"""	Recebe um goban e uma cadeia e remove as pedras de todas as suas interseções."""

	for interseção in t:
		remove_pedra(g, interseção)

	return g


def eh_goban(arg:any) -> bool:
	"""	Recebe um argumento e verifica se é um goban."""

	if not isinstance(arg, list):
		return False
	if len(arg) not in (9, 13, 19):
		return False

	for x in arg:
		if not isinstance(x, list):
			return False
		if len(x) != len(arg):
			return False
		for y in x:
			if not eh_pedra(y):
				return False
	
	return True


def eh_intersecao_valida(g:'list[list[str]]', i:'tuple[str,int]') -> bool:
	""" Recebe um goban e uma intersecao e verifica se a intersecao é valida nesse goban."""

	last = obtem_ultima_intersecao(g)

	if not eh_goban(g):
		return False
	if not eh_intersecao(i):
		return False
	if not obtem_col(i) <= obtem_col(last):
		return False
	if not obtem_lin(i) <= obtem_lin(last):
		return False
	
	return True


def gobans_iguais(g1:'list[list[str]]', g2:'list[list[str]]') -> bool:
	"""	Recebe dois gobans e verifica se são iguais."""

	if not eh_goban(g1) or not eh_goban(g2):
		return False
	
	return g1 == g2


def goban_para_str(g:'list[list[str]]') -> str:
	""" Recebe um goban e devolve uma cadeia de caracteres que o representa."""
	if not eh_goban(g):
		raise ValueError('territorio_para_str: argumento invalido')

	# Dimensoes do territorio
	cols = len(g)
	rows = len(g[0])
	field = "  "

	# Primeira linha
	for i in range(cols):
		field += " " + caracter("A", i)
	field += "\n"

	# Corpo
	for i in range(rows, 0, -1):
		field += " " if i < 10 else ""
		field += str(i) + " "
		for j in range(cols):
			field += pedra_para_str(obtem_pedra(g, cria_intersecao(caracter("A", j), i))) + " "
		field += " " if i < 10 else ""
		field += str(i) + "\n"

	# Ultima linha
	field += "  "
	for i in range(cols):
		field += " " + caracter("A", i)

	return field


def obtem_intersecoes_neutras(g:'list[list[str]]') -> 'list[tuple[str,int]]':
	"""	Recebe um goban e devolve uma lista com as interseções que têm pedras neutras."""

	# Intersecoes neutras
	res = []

	for col in range(len(g)):
		for lin in range(len(g)):
			interseção = cria_intersecao(caracter("A", col), lin + 1)
			if not eh_pedra_jogador(obtem_pedra(g, interseção)):
				res.append(interseção)

	return res


def obtem_territorios(g:'list[list[str]]') -> 'tuple[tuple[tuple[str,int]]]':
	"""	Devolve o tuplo formado pelos tuplos com as interseções de cada território de g."""
	
	# Intersecoes neutras
	stack = obtem_intersecoes_neutras(g)
	# Territorios
	territorios = []

	while len(stack) > 0:
		current = stack.pop()
		cadeia  = obtem_cadeia(g, current)
		territorios.append(cadeia)
		stack   = [interseção for interseção in stack if interseção not in cadeia]

	return tuple(sorted(territorios, key=lambda x: (obtem_lin(x[0]), obtem_col(x[0]))))


def obtem_adjacentes_diferentes(g:'list[list[str]]', t:'tuple[tuple[str,int]]') -> tuple[tuple[str,int]]:
	"""	Devolve o tuplo ordenado formado pelas interseções diferentes adjacentes
		às interseções do tuplo t. """
	
	last = obtem_ultima_intersecao(g)

	# Intersecoes diferentes adjacentes
	res = []

	for interseção in t:
		for adjacente in obtem_intersecoes_adjacentes(interseção, last):
			if	eh_pedra_jogador(obtem_pedra(g, adjacente)) != eh_pedra_jogador(obtem_pedra(g, interseção))\
				and adjacente not in res:
				res.append(adjacente)

	return ordena_intersecoes(tuple(res))


def tem_liberdades(g:'list[list[str]]', i:'tuple[str, int]', p:str) -> bool:
	"""	Determina se a cadeia de i do goban g tem liberdades a favor do jogador p."""

	for adjacente in obtem_adjacentes_diferentes(g, obtem_cadeia(g, i)):
		if obtem_pedra(g, adjacente) != inimigo(p):
			return True
		
	return False


def inimigo(p:str) -> str:
	""" Recebe uma pedra de um jogador e devolve a pedra do jogador contrário."""

	return cria_pedra_branca() if eh_pedra_preta(p) else cria_pedra_preta()


def jogada(g:'list[list[str]]', i:'tuple[str,int]', p:str) -> 'list[list[str]]':
	""" Modifica destrutivamente o goban g colocando a pedra de jogador p na
		interseção i e remove todas as pedras do jogador contrário pertencentes
		a cadeias adjacentes à i sem liberdades, devolvendo o próprio goban."""

	coloca_pedra(g, i, p)
	for adjacente in obtem_intersecoes_adjacentes(i, obtem_ultima_intersecao(g)):
		if pedras_iguais(obtem_pedra(g, adjacente), inimigo(p)):
			if not tem_liberdades(g, adjacente, inimigo(p)):
				remove_cadeia(g, obtem_cadeia(adjacente))

	return g


def obtem_pedras_jogadores(g:'list[list[str]]') -> 'tuple[int,int]':
	""" Devolve um tuplo de dois inteiros que correspondem ao número de interseções
		ocupadas por pedras do jogador branco e preto, respetivamente."""

	branco = 0
	preto  = 0

	last = obtem_ultima_intersecao(g)
	for col in range(indice(obtem_col(last)) + 1):
		for lin in range(obtem_lin(last)):
			interseção = cria_intersecao(caracter("A", col), lin + 1)
			if eh_pedra_branca(obtem_pedra(g, interseção)):
				branco += 1
			if eh_pedra_preta(obtem_pedra(g, interseção)):
				preto  += 1


	return (branco, preto)


def eh_territorio_jogador(g:'list[list[str]]', t:'tuple[tuple[str,int]]', p:str) -> bool:
	""" Recebe um goban, um território e uma pedra de jogador e verifica se o
		território é do jogador."""
	
	fronteira = obtem_adjacentes_diferentes(g, t)
	if fronteira == ():
		return False

	for interseção in fronteira:
		if obtem_pedra(g, interseção) != p:
			return False
		
	return True


""" Funções Adicionais."""
def calcula_pontos(g:'list[list[str]]') -> 'tuple[int,int]':
	""" Recebe um goban e devolve o tuplo de dois inteiros com as pontuações dos
		jogadores branco e preto, respetivamente."""
	
	pedras = obtem_pedras_jogadores(g)

	branco = pedras[0]
	preto  = pedras[1]
	for território in obtem_territorios(g):
		if eh_territorio_jogador(g, território, cria_pedra_branca()):
			branco += len(território)
		if eh_territorio_jogador(g, território, cria_pedra_preta()):
			preto  += len(território)

	return (branco, preto)


def eh_jogada_legal(g:'list[list[str]]', i:'tuple[str,int]', p:str, l:'list[list[str]]') -> bool:
	""" Recebe um goban g, uma interseção i, uma pedra de jogador p e um outro
		goban l e devolve True se a jogada for legal ou False caso contrário, sem
		modificar g ou l."""

	# Fora do tabuleiro
	if not eh_intersecao_valida(g, i):
		return False

	# Sobreposição
	if eh_pedra_jogador(obtem_pedra(g, i)):
		return False

	goban = cria_copia_goban(g)
	jogada(goban, i, p)

	# Repetição
	if gobans_iguais(goban, l):
		return False
	
	# Suicídio
	if not tem_liberdades(goban, i, p):
		return False
	
	return True


def turno_jogador(g:'list[list[str]]', p:str, l:'list[list[str]]') -> bool:
	""" Recebe um goban g, uma pedra de jogador p e um outro goban l, e oferece
		ao jogador que joga com pedras p a opção de passar ou de colocar uma pedra
		própria numa interseção. Devolve False se o jogador passar ou True se
		colocar uma pedra."""
	
	turno_atual = ""
	while not ((
			eh_str_intersecao(turno_atual) and eh_jogada_legal(g, str_para_intersecao(turno_atual), p, l)
		) or (
			turno_atual == "P"
		)):
		turno_atual = input("Escreva uma intersecao ou 'P' para passar [{}]:".format(pedra_para_str(p)))

	if turno_atual == "P":
		return False
	
	l = cria_copia_goban(g)
	jogada(g, str_para_intersecao(turno_atual), p)

	return True

def jogo_para_str(g:'list[list[str]]', t:'[int, int]') -> str:
	""" Recebe um goban e um tuplo com as pontuações dos jogadores branco e preto
		e retorna uma cadeia de caracteres que representa o jogo."""
	return	"Branco (O) tem {} pontos\n".format(t[0]) + \
			"Preto (X) tem {} pontos\n" .format(t[1]) + \
			goban_para_str(g)


def go(n:int, tb:'tuple[tuple[str,int]]', tn:'tuple[tuple[str,int]]') -> bool:
	""" Função principal que permite jogar um jogo completo do Go de dois jogadores.
		A função recebe um inteiro correspondente à dimensão do tabuleiro, e dois
		tuplos com a representação externa das interseções ocupadas inicialmente. """
	
	# Verificação dos argumentos
	if not (isinstance(n, int) and isinstance(tb, tuple) and isinstance(tn, tuple)):
		raise ValueError("go: argumentos invalidos")
	if n not in (9, 13, 19):
		raise ValueError("go: argumentos invalidos")
	if not all(eh_str_intersecao(x) for x in tb):
		raise ValueError("go: argumentos invalidos")
	if not all(eh_str_intersecao(x) for x in tn):
		raise ValueError("go: argumentos invalidos")
	
	# Criação do goban
	vazio = cria_goban_vazio(n)
	if not all(eh_intersecao_valida(vazio, str_para_intersecao(x)) for x in tb):
		raise ValueError("go: argumentos invalidos")
	
	# Criação do goban.
	pedras_brancas = tuple(str_para_intersecao(x) for x in tb)
	pedras_pretas  = tuple(str_para_intersecao(x) for x in tn)
	goban		   = cria_goban(n, pedras_brancas, pedras_pretas)
	goban_anterior = cria_copia_goban(goban)

	# Turnos. Começa o preto.
	turno_atual	   = cria_pedra_preta()
	turno_anterior = cria_pedra_branca()

	# Indicam se a jogada atual e/ou anterior foram passadas.
	passe_atual	   = False
	passe_anterior = False

	# Loop de Jogo.
	while not (passe_atual and passe_anterior):
		passe_anterior = passe_atual
		pontos		   = calcula_pontos(goban)
		print(jogo_para_str(goban, pontos))
		passe_atual	   = not turno_jogador(goban, turno_atual, goban_anterior)

		turno_atual, turno_anterior = turno_anterior, turno_atual

	# Fim de Jogo.
	print(jogo_para_str(goban, pontos))
	pontos = calcula_pontos(goban)

	return pontos[0] >= pontos[1]