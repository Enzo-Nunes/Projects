alfabeto = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'


def eh_territorio(arg):
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


def obtem_ultima_intersecao(t):
	return 	alfabeto[len(t)-1], len(t[0])


def eh_intersecao(arg):
	if not isinstance(arg, tuple):
		return False
	if not isinstance(arg[0], str):
		return False
	if not isinstance(arg[1], int):
		return False
	return True


def eh_intersecao_valida(t, i):
	if not eh_territorio(t):
		return False
	if not eh_intersecao(i):
		return False
	if 'A' <= i[0] <= alfabeto[len(t)-1] and 1 <= i[1] <= len(t[0]):
		return True
	return False


def eh_intersecao_livre(t, i):
	if not eh_intersecao_valida(t, i):
		return False
	if t[alfabeto.index(i[0])][i[1]-1] == 0:
		return True
	return False


def obtem_intersecoes_adjacentes(t, i):

	# Limits
	upper = len(t[0]) + 1
	lower = 0
	left = ""
	right = alfabeto[len(t)]

	# Adjacent Positions
	col_esquerda	= alfabeto[alfabeto.index(i[0]) - 1] if i[0] != "A" else ""
	col_direita		= alfabeto[alfabeto.index(i[0]) + 1] if i[0] != "Z" else ""
	lin_baixo		= i[1] - 1
	lin_cima		= i[1] + 1

	# Adjacent Intersections
	esquerda = (col_esquerda, i[1])	if col_esquerda != left	 else ()
	direita	 = (col_direita, i[1])	if col_direita	!= right else ()
	baixo	 = (i[0], lin_baixo)	if lin_baixo	!= lower else ()
	cima	 = (i[0], lin_cima)		if lin_cima		!= upper else ()

	return tuple(x for x in (baixo, esquerda, direita, cima) if x != ())


def ordena_intersecoes(tup):
	return tuple(sorted(tup, key=lambda x: (x[1], x[0])))


def territorio_para_str(t):
	if not eh_territorio(t):
		raise ValueError('territorio_para_str: argumento invalido')
	
	cols = len(t)
	rows = len(t[0])
	field = "  "

	for i in range(cols):
		field += " " + alfabeto[i]
	field += "\n"

	for i in range(rows, 0, -1):
		field += " " if i < 10 else ""
		field += str(i) + " "
		for j in range(cols):
			field += ". " if eh_intersecao_livre(t, (alfabeto[j], i)) else "X "
		field += " " if i < 10 else ""
		field += str(i) + "\n"

	field += "  "
	for i in range(cols):
		field += " " + alfabeto[i]

	return field


def obtem_cadeia(t, i):
	if not eh_intersecao_valida(t, i):
		raise ValueError('obtem_cadeia: argumentos invalidos')

	toCheck = [i]
	checked = []

	while len(toCheck) > 0:
		current = toCheck.pop()
		if current not in checked:
			checked.append(current)
			for x in obtem_intersecoes_adjacentes(t, current):
				if x not in checked and x not in toCheck and eh_intersecao_livre(t, x) == eh_intersecao_livre(t, i):
					toCheck.append(x)
	
	return ordena_intersecoes(tuple(checked))
		

def obtem_vale(t, i):
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


def verifica_conexao(t, i1, i2):
	if not eh_intersecao_valida(t, i1) or not eh_intersecao_valida(t, i2):
		raise ValueError('verifica_conexao: argumentos invalidos')
	return i2 in obtem_cadeia(t, i1) and eh_intersecao_livre(t, i1) == eh_intersecao_livre(t, i2)


def calcula_numero_montanhas(t):
	if not eh_territorio(t):
		raise ValueError('calcula_numero_montanhas: argumento invalido')
	res = 0
	for x in t:
		for y in x:
			res += y
	return res


def getMontanhas(t):
	res = []
	for i in range(len(t)):
		for j in range(len(t[i])):
			if not eh_intersecao_livre(t, (alfabeto[i], j+1)):
				res.append((alfabeto[i], j+1))
	return res


def calcula_numero_cadeias_montanhas(t):
	if not eh_territorio(t):
		raise ValueError('calcula_numero_cadeias_montanhas: argumento invalido')
	
	res = 0
	checked = []
	toCheck = getMontanhas(t)

	while len(toCheck) > 0:
		current = toCheck.pop()
		for x in obtem_cadeia(t, current):
			if x not in checked:
				checked.append(x)
			if x in toCheck:
				toCheck.remove(x)
		res += 1
	return res


def calcula_tamanho_vales(t):
	if not eh_territorio(t):
		raise ValueError('calcula_tamanho_vales: argumento invalido')
	
	toCheck = getMontanhas(t)
	vales = []

	while len(toCheck) > 0:
		current = toCheck.pop()
		for x in obtem_vale(t, current):
			if x not in vales:
				vales.append(x)

	return len(vales)