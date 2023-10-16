ALFABETO = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

def eh_territorio(arg) -> bool:
	"""	Recebe um argumento de qualquer tipo e devolve True se o seu argumento
		corresponde a um territorio e False caso contrario."""
	
	if not isinstance(arg, tuple):
		return False
	for x in arg:
		if not isinstance(x, tuple):
			return False
		if len(x) != len(arg[0]):
			return False
		for y in x:
			if not isinstance(y, int):
				return False
			if y != 0 and y != 1:
				return False
			
	return True


def obtem_ultima_intersecao(t:tuple) -> tuple:
	""" Recebe um territorio e devolve a intersecao do extremo superior
		direito do territorio."""
	
	if not eh_territorio(t):
		raise ValueError('obtem_ultima_intersecao: argumento invalido')
	
	return ALFABETO[len(t)-1], len(t[0])


def eh_intersecao(arg):
	""" Recebe um argumento de qualquer tipo e devolve True se o seu argumento
		corresponde a uma intersecao e False caso contrario."""
	
	if not isinstance(arg, tuple):
		return False
	
	if len(arg) != 2:
		return False
	if not isinstance(arg[0], str):
		return False
	if arg[0] not in ALFABETO:
		return False
	
	if not isinstance(arg[1], int):
		return False
	if arg[1] < 1 or arg[1] > 99:
		return False

	return True


def eh_intersecao_valida(t:tuple, i:tuple) -> bool:
	""" Recebe um territorio e uma intersecao e devolve True se o seu argumento
		corresponde a uma intersecao valida nesse territorio e False caso contrario."""

	last = obtem_ultima_intersecao(t)

	if not eh_territorio(t):
		return False
	if not eh_intersecao(i):
		return False
	if not i[0] <= last[0]:
		return False
	if not i[1] <= last[1]:
		return False
	
	return True


def eh_intersecao_livre(t:tuple, i:tuple) -> bool:
	""" Recebe um territorio e uma intersecao e devolve True se o seu argumento
		corresponde a uma intersecao sem montanha nesse territorio e False caso contrario."""

	if not eh_intersecao_valida(t, i):
		return False
	
	return not t[ALFABETO.index(i[0])][i[1]-1]


def obtem_intersecoes_adjacentes(t:tuple, i:tuple) -> tuple:
	""" Recebe um territorio e uma intersecao e devolve uma tuplo com as intersecoes
		validas adjacentes da intersecao dada."""

	# Limites do Territorio
	lower = 1
	left  = "A"
	right = obtem_ultima_intersecao(t)[0]
	upper = obtem_ultima_intersecao(t)[1]

	# Posições adjacentes
	esquerda = (ALFABETO[ALFABETO.index(i[0]) - 1], i[1]) if i[0] != left  else ()
	direita	 = (ALFABETO[ALFABETO.index(i[0]) + 1], i[1]) if i[0] != right else ()
	baixo	 = (ALFABETO[ALFABETO.index(i[0])], i[1] - 1) if i[1] != lower else ()
	cima	 = (ALFABETO[ALFABETO.index(i[0])], i[1] + 1) if i[1] != upper else ()

	return tuple(x for x in (baixo, esquerda, direita, cima) if x != ())


def ordena_intersecoes(tup:tuple) -> tuple:
	""" Recebe um tuplo de intersecoes e devolve um tuplo com as intersecoes
		ordenadas de acordo com a ordem de leitura do territorio."""
	
	return tuple(sorted(tup, key=lambda x: (x[1], x[0])))


def territorio_para_str(t:tuple) -> str:
	""" Recebe um territorio e devolve uma cadeia de caracteres que o representa."""

	if not eh_territorio(t):
		raise ValueError('territorio_para_str: argumento invalido')

	# Dimensoes do territorio
	cols = len(t)
	rows = len(t[0])
	field = "  "

	# Primeira linha
	for i in range(cols):
		field += " " + ALFABETO[i]
	field += "\n"

	# Corpo
	for i in range(rows, 0, -1):
		field += " " if i < 10 else ""
		field += str(i) + " "
		for j in range(cols):
			field += ". " if eh_intersecao_livre(t, (ALFABETO[j], i)) else "X "
		field += " " if i < 10 else ""
		field += str(i) + "\n"

	# Ultima linha
	field += "  "
	for i in range(cols):
		field += " " + ALFABETO[i]

	return field


def obtem_cadeia(t:tuple, i:tuple) -> tuple:
	""" Recebe um territorio e uma intersecao e devolve um tuplo ordenado com
		todas as intersecoes connectadas a i."""
	
	if not eh_intersecao_valida(t, i):
		raise ValueError('obtem_cadeia: argumentos invalidos')

	# Intersecoes a verificar
	stack = [i]
	# Intersecoes verificadas (resultado)
	res = []

	# Algoritmo de busca em profundidade.
	while len(stack) > 0:
		current = stack.pop()
		if current not in res:
			res.append(current)
			for x in obtem_intersecoes_adjacentes(t, current):
				if x not in res and x not in stack and\
				   eh_intersecao_livre(t, x) == eh_intersecao_livre(t, i):
					stack.append(x)

	return ordena_intersecoes(tuple(res))


def obtem_vale(t:tuple, i:tuple) -> tuple:
	""" Recebe um territorio e uma intersecao e devolve um tuplo ordenado com
		todas as intersecoes livres adjacentes a cadeia de i."""
	
	if not eh_intersecao_valida(t, i):
		raise ValueError('obtem_vale: argumentos invalidos')
	if eh_intersecao_livre(t, i):
		raise ValueError('obtem_vale: argumentos invalidos')

	vale = []
	for x in obtem_cadeia(t, i):
		for y in obtem_intersecoes_adjacentes(t, x):
			if y not in vale and eh_intersecao_livre(t, y):
				vale.append(y)

	return ordena_intersecoes(tuple(vale))


def verifica_conexao(t:tuple, i1:tuple, i2:tuple) -> bool:
	""" Recebe um territorio e duas intersecoes e devolve True se as intersecoes
		pertencem a uma mesma cadeia e False caso contrario."""
	
	if not eh_intersecao_valida(t, i1) or not eh_intersecao_valida(t, i2):
		raise ValueError('verifica_conexao: argumentos invalidos')
	
	return i2 in obtem_cadeia(t, i1)


def calcula_numero_montanhas(t:tuple) -> int:
	""" Recebe um territorio e devolve o numero de montanhas do territorio."""

	if not eh_territorio(t):
		raise ValueError('calcula_numero_montanhas: argumento invalido')
	
	res = 0
	for x in t:
		for y in x:
			res += y

	return res


def obtem_montanhas(t:tuple) -> list:
	""" Recebe um territorio e devolve uma lista com todas as suas intersecoes
		que	contem montanhas."""
	
	res = []
	for i in range(len(t)):
		for j in range(len(t[i])):
			if not eh_intersecao_livre(t, (ALFABETO[i], j+1)):
				res.append((ALFABETO[i], j+1))

	return res


def calcula_numero_cadeias_montanhas(t:tuple) -> int:
	""" Recebe um territorio e devolve o numero de cadeias de montanhas do territorio."""

	if not eh_territorio(t):
		raise ValueError('calcula_numero_cadeias_montanhas: argumento invalido')

	# Numero de Cadeias de montanhas
	res = 0
	# Intersecoes com montanhas
	stack = obtem_montanhas(t)

	while len(stack) > 0:
		current = stack.pop()
		for x in obtem_cadeia(t, current):
			if x in stack:
				stack.remove(x)
		res += 1

	return res


def calcula_tamanho_vales(t:tuple) -> int:
	""" Recebe um territorio e devolve o numero de intersecoes livres que
		pertencem a vales do territorio."""
	
	if not eh_territorio(t):
		raise ValueError('calcula_tamanho_vales: argumento invalido')

	vales = []
	stack = obtem_montanhas(t)

	while len(stack) > 0:
		current = stack.pop()
		for x in obtem_vale(t, current):
			if x not in vales:
				vales.append(x)

	return len(vales)