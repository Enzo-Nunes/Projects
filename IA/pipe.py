# pipe.py: Template para implementação do projeto de Inteligência Artificial 2023/2024.
# Devem alterar as classes e funções neste ficheiro de acordo com as instruções do enunciado.
# Além das funções e classes sugeridas, podem acrescentar outras que considerem pertinentes.

# Grupo 38:
# 106588 Tomás Barros
# 106336 Enzo Nunes

import sys
import cProfile
import numpy as np
from search import (
	Problem,
	Node,
	astar_search,
	breadth_first_tree_search,
	depth_first_tree_search,
	greedy_search,
	recursive_best_first_search,
)

#Peças
FC = 0
FD = 1
FB = 2
FE = 3

BC = 4
BD = 5
BB = 6
BE = 7

VC = 8
VD = 9
VB = 10
VE = 11

LH = 12
LV = 13


class Piece:
	""" Representação interna de uma peça de PipeMania. """

	def __init__(self, type: int,):
		self.type = type
		self.locked = False # True se a peça não pode ser rodada, False caso contrário

	@staticmethod
	def parse_piece(piece: str) -> "Piece":
		"""	Recebe uma string que representa uma peça e devolve um objeto da classe Piece correspondente."""
		if piece[0] == "F":
			if piece[1] == "C":
				return Piece(FC)
			elif piece[1] == "D":
				return Piece(FD)
			elif piece[1] == "B":
				return Piece(FB)
			elif piece[1] == "E":
				return Piece(FE)
		elif piece[0] == "B":
			if piece[1] == "C":
				return Piece(BC)
			elif piece[1] == "D":
				return Piece(BD)
			elif piece[1] == "B":
				return Piece(BB)
			elif piece[1] == "E":
				return Piece(BE)
		elif piece[0] == "V":
			if piece[1] == "C":
				return Piece(VC)
			elif piece[1] == "D":
				return Piece(VD)
			elif piece[1] == "B":
				return Piece(VB)
			elif piece[1] == "E":
				return Piece(VE)
		elif piece[0] == "L":
			if piece[1] == "H":
				return Piece(LH)
			elif piece[1] == "V":
				return Piece(LV)
	
	def __str__(self) -> str:
		"""	Converte a peça para uma string no formato pedido."""
		if self.type == FC:
			return "FC"
		elif self.type == FD:
			return "FD"
		elif self.type == FB:
			return "FB"
		elif self.type == FE:
			return "FE"
		elif self.type == BC:
			return "BC"
		elif self.type == BD:
			return "BD"
		elif self.type == BB:
			return "BB"
		elif self.type == BE:
			return "BE"
		elif self.type == VC:
			return "VC"
		elif self.type == VD:
			return "VD"
		elif self.type == VB:
			return "VB"
		elif self.type == VE:
			return "VE"
		elif self.type == LH:
			return "LH"
		elif self.type == LV:
			return "LV"

	def is_F(self) -> bool:
		""" Verifica se a peça é um fecho."""
		return FC <= self.type <= FE
	
	def is_B(self) -> bool:
		""" Verifica se a peça é uma bifurcação."""
		return BC <= self.type <= BE

	def is_V(self) -> bool:
		""" Verifica se a peça é uma volta."""
		return VC <= self.type <= VE
	
	def is_L(self) -> bool:
		""" Verifica se a peça é uma ligação."""
		return LH <= self.type <= LV

	def connect_up(self) -> bool:
		"""	Verifica se uma peça pode conectar-se a peças acima."""
		return self.type in [FC, BC, BE, BD, VC, VD, LV]

	def connect_right(self) -> bool:
		"""	Verifica se uma peça pode conectar-se a peças da direita."""
		return self.type in [FD, BC, BB, BD, VB, VD, LH]
	
	def connect_down(self) -> bool:
		"""	Verifica se uma peça pode conectar-se a peças abaixo."""
		return self.type in [FB, BB, BE, BD, VB, VE, LV]

	def connect_left(self) -> bool:
		"""	Verifica se uma peça pode conectar-se a peças da esquerda."""
		return self.type in [FE, BC, BB, BE, VC, VE, LH]

	def lock(self):
		"""	Bloqueia a peça, impedindo-a de ser rodada."""
		self.locked = True

	def pretty_str(self) -> "list[str, str, str]":
		""" Devolve uma lista de strings que representam a peça de forma mais legível. Ajuda a debuggar."""
		if self.type == FC:
			return [" | ", " 0 ", "   "]
		elif self.type == FD:
			return ["   ", " 0-", "   "]
		elif self.type == FB:
			return ["   ", " 0 ", " | "]
		elif self.type == FE:
			return ["   ", "-0 ", "   "]
		elif self.type == BC:
			return [" | ", "---", "   "]
		elif self.type == BD:
			return [" | ", " |-", " | "]
		elif self.type == BB:
			return ["   ", "---", " | "]
		elif self.type == BE:
			return [" | ", "-| ", " | "]
		elif self.type == VC:
			return [" | ", "-- ", "   "]
		elif self.type == VD:
			return [" | ", " --", "   "]
		elif self.type == VB:
			return ["   ", " --", " | "]
		elif self.type == VE:
			return ["   ", "-- ", " | "]
		elif self.type == LH:
			return ["   ", "---", "   "]
		elif self.type == LV:
			return [" | ", " | ", " | "]

class Board:
	"""Representação interna de um tabuleiro de PipeMania."""

	def __init__(self, matrix:np.ndarray, size: int):
		self.matrix = matrix
		self.size = size
		self.disconnections = self.calculate_disconnections()

	def get_value(self, row: int, col: int) -> Piece:
		"""	Devolve a peça na posição (row, col) do tabuleiro."""
		return self.matrix[row][col]

	def set_value(self, row: int, col: int, value: Piece):
		"""	Atribui a peça 'value' à posição (row, col) do tabuleiro"""
		self.matrix[row][col] = value

	def is_top_row(self, row: int) -> bool:
		"""	Verifica se a linha passada como argumento é a primeira do tabuleiro."""
		return row == 0

	def is_bot_row(self, row: int) -> bool:
		"""	Verifica se a linha passada como argumento é a última do tabuleiro."""
		return row == self.size - 1

	def is_left_col(self, col: int) -> bool:
		"""	Verifica se a coluna passada como argumento é a primeira do tabuleiro."""
		return col == 0

	def is_right_col(self, col: int) -> bool:
		"""	Verifica se a coluna passada como argumento é a última do tabuleiro."""
		return col == self.size - 1

	def adjacent_vertical_values(self, row: int, col: int) -> "tuple[Piece, Piece]":
		"""	Devolve as peças acima e abaixo da peça na posição (row, col)."""
		if self.is_top_row(row):
			return (None, self.get_value(row + 1, col))
		elif self.is_bot_row(row):
			return (self.get_value(row - 1, col), None)
		else:
			return (self.get_value(row - 1, col), self.get_value(row + 1, col))

	def adjacent_horizontal_values(self, row: int, col: int) -> "tuple[Piece, Piece]":
		"""	Devolve as peças à esquerda e à direita da peça na posição (row, col)."""
		if self.is_left_col(col):
			return (None, self.get_value(row, col + 1))
		elif self.is_right_col(col):
			return (self.get_value(row, col - 1), None)
		else:
			return (self.get_value(row, col - 1), self.get_value(row, col + 1))

	def get_next(self, current: "tuple[int, int]") -> "tuple[int, int]":
		"""	Devolve as coordenadas no tabuleiro seguintes às coordenadas passadas como argumento."""
		row, col = current
		if col == self.size - 1:
			if row == self.size - 1:
				return (0, 0)
			return (row + 1, 0)
		return (row, col + 1)

	def calculate_disconnections(self) -> int:
		"""	Calcula o número de desconexões do tabuleiro."""
		disconnections = 0
		for row in range(self.size):
			for col in range(self.size):
				piece = self.get_value(row, col)
				piece_down = self.adjacent_vertical_values(row, col)[1]
				piece_right = self.adjacent_horizontal_values(row, col)[1]

				# Adiciona desconexões se a peça apontar para uma borda
				if self.is_top_row(row) and piece.connect_up():
					disconnections += 1
				if self.is_bot_row(row) and piece.connect_down():
					disconnections += 1
				if self.is_left_col(col) and piece.connect_left():
					disconnections += 1
				if self.is_right_col(col) and piece.connect_right():
					disconnections += 1

				# Adiciona desconexões se a peça estiver desconectada dos vizinhos inferior e direito
				if piece_down and ((piece.connect_down() and not piece_down.connect_up()) or (not piece.connect_down() and piece_down.connect_up())):
					disconnections += 1
				if piece_right and ((piece.connect_right() and not piece_right.connect_left()) or (not piece.connect_right() and piece_right.connect_left())):
					disconnections += 1
		return disconnections

	def get_adjacent_disconnections(self, row: int, col: int) -> int:
		"""	Devolve o número de desconexões da peça na posição (row, col) com as peças adjacentes."""
		disconnections = 0
		piece = self.get_value(row, col)
		piece_up, piece_down = self.adjacent_vertical_values(row, col)
		piece_left, piece_right = self.adjacent_horizontal_values(row, col)

		# Adiciona desconexões se a peça apontar para uma borda
		if self.is_top_row(row) and piece.connect_up():
			disconnections += 1
		if self.is_bot_row(row) and piece.connect_down():
			disconnections += 1
		if self.is_left_col(col) and piece.connect_left():
			disconnections += 1
		if self.is_right_col(col) and piece.connect_right():
			disconnections += 1
		
		# Determian se a peça está desconectada das duas peças adjacentes
		if piece_up and ((piece.connect_up() and not piece_up.connect_down()) or (not piece.connect_up() and piece_up.connect_down())):
			disconnections += 1
		if piece_down and ((piece.connect_down() and not piece_down.connect_up()) or (not piece.connect_down() and piece_down.connect_up())):
			disconnections += 1
		if piece_left and ((piece.connect_left() and not piece_left.connect_right()) or (not piece.connect_left() and piece_left.connect_right())):
			disconnections += 1
		if piece_right and ((piece.connect_right() and not piece_right.connect_left()) or (not piece.connect_right() and piece_right.connect_left())):
			disconnections += 1
		
		return disconnections

	def get_actions(self, row: int, col: int):
		"""	Devolve as melhores jogadas para a peça na posição (row, col) do tabuleiro de acordo com o objetivo e
		posição atual do jogo. Inclui todas as restrições que limitam a rotação de peças."""
		current_piece = self.get_value(row, col)

		# Se a peça estiver bloqueada, não é possível rodá-la
		if current_piece.locked:
			return []
		
		# Peças adjacentes
		piece_left, piece_right = self.adjacent_horizontal_values(row, col)
		piece_up, piece_down = self.adjacent_vertical_values(row, col)
		
		# Fecho
		if current_piece.is_F():
			if piece_up and piece_up.connect_down() and piece_up.locked and not piece_up.is_F():
				return [Piece(FC)]
			if piece_down and piece_down.connect_up() and piece_down.locked and not piece_down.is_F():
				return [Piece(FB)]
			if piece_right and piece_right.connect_left() and piece_right.locked and not piece_right.is_F():
				return [Piece(FD)]
			if piece_left and piece_left.connect_right() and piece_left.locked and not piece_left.is_F():
				return [Piece(FE)]
			
			counter = 0	# Conta o número de desconexões do fecho.
			if piece_up and piece_up.locked and not piece_up.connect_down():
				counter += 1
			if piece_right and piece_right.locked and not piece_right.connect_left():
				counter += 1
			if piece_down and piece_down.locked and not piece_down.connect_up():
				counter += 1
			if piece_left and piece_left.locked and not piece_left.connect_right():
				counter += 1
			if counter == 3:
				if piece_up and not piece_up.locked:
					return [Piece(FC)]
				if piece_down and not piece_down.locked:
					return [Piece(FB)]
				if piece_right and not piece_right.locked:
					return [Piece(FD)]
				if piece_left and not piece_left.locked:
					return [Piece(FE)]

			actions = []
			if piece_up and (piece_up.connect_down() or not piece_up.locked) and not piece_up.is_F():
				actions.append(Piece(FC))
			if piece_down and (piece_down.connect_up() or not piece_down.locked) and not piece_down.is_F():
				actions.append(Piece(FB))
			if piece_right and (piece_right.connect_left() or not piece_right.locked) and not piece_right.is_F():
				actions.append(Piece(FD))
			if piece_left and (piece_left.connect_right() or not piece_left.locked) and not piece_left.is_F():
				actions.append(Piece(FE))
			return actions
		
		# Bifurcação
		if current_piece.is_B():
			
			if not piece_up or (not piece_up.connect_down() and piece_up.locked):
				return [Piece(BB)]
			if not piece_down or (not piece_down.connect_up() and piece_down.locked):
				return [Piece(BC)]
			if not piece_right or (not piece_right.connect_left() and piece_right.locked):
				return [Piece(BE)]
			if not piece_left or (not piece_left.connect_right() and piece_left.locked):
				return [Piece(BD)]

			actions = [BC, BD, BB, BE]
			if piece_up.connect_down() and piece_up.locked:
				actions.remove(BB)
			if piece_down.connect_up() and piece_down.locked:
				actions.remove(BC)
			if piece_right.connect_left() and piece_right.locked:
				actions.remove(BE)
			if piece_left.connect_right() and piece_left.locked:
				actions.remove(BD)
		
			return [Piece(action) for action in actions]
		
		# Volta
		if current_piece.is_V():
			if not piece_up or (piece_up.locked and not piece_up.connect_down()):
				if not piece_left or (piece_left.locked and not piece_left.connect_right()) or (piece_right and piece_right.locked and piece_right.connect_left()):
					return [Piece(VB)]
				if not piece_right or (piece_right.locked and not piece_right.connect_left()) or (piece_left and piece_left.locked and piece_left.connect_right()):
					return [Piece(VE)]
			if not piece_down or (piece_down.locked and not piece_down.connect_up()):
				if not piece_left or (piece_left.locked and not piece_left.connect_right()) or (piece_right and piece_right.locked and piece_right.connect_left()):
					return [Piece(VD)]
				if not piece_right or (piece_right.locked and not piece_right.connect_left()) or (piece_left and piece_left.locked and piece_left.connect_right()):
					return [Piece(VC)]
				
			if piece_up and piece_left and piece_up.locked and piece_up.connect_down() and piece_left.locked and piece_left.connect_right():
				return [Piece(VC)]
			if piece_up and piece_right and piece_up.locked and piece_up.connect_down() and piece_right.locked and piece_right.connect_left():
				return [Piece(VD)]
			if piece_down and piece_left and piece_down.locked and piece_down.connect_up() and piece_left.locked and piece_left.connect_right():
				return [Piece(VE)]
			if piece_down and piece_right and piece_down.locked and piece_down.connect_up() and piece_right.locked and piece_right.connect_left():
				return [Piece(VB)]
			
			if not piece_left or (piece_left.locked and not piece_left.connect_right()):
				if piece_up and piece_up.locked and piece_up.connect_down():
					return [Piece(VD)]
				if piece_down and piece_down.locked and piece_down.connect_up():
					return [Piece(VB)]
			if not piece_right or (piece_right.locked and not piece_right.connect_left()):
				if piece_up and piece_up.locked and piece_up.connect_down():
					return [Piece(VC)]
				if piece_down and piece_down.locked and piece_down.connect_up():
					return [Piece(VE)]
			
			actions = [VC, VD, VB, VE]
			if not piece_up or (not piece_up.connect_down() and piece_up.locked) or not piece_left or (not piece_left.connect_right() and piece_left.locked):
				actions.remove(VC)
			if not piece_up or (not piece_up.connect_down() and piece_up.locked) or not piece_right or (not piece_right.connect_left() and piece_right.locked):
				actions.remove(VD)
			if not piece_down or (not piece_down.connect_up() and piece_down.locked) or not piece_left or (not piece_left.connect_right() and piece_left.locked):
				actions.remove(VE)
			if not piece_down or (not piece_down.connect_up() and piece_down.locked) or not piece_right or (not piece_right.connect_left() and piece_right.locked):
				actions.remove(VB)
			return [Piece(action) for action in actions]
		
		# Ligação
		if current_piece.is_L():
			if not piece_up or not piece_down or (piece_up.locked and not piece_up.connect_down()) or (piece_down.locked and not piece_down.connect_up()) or (piece_right and piece_right.locked and piece_right.connect_left()) or (piece_left and piece_left.locked and piece_left.connect_right()):
				return [Piece(LH)]
			if not piece_left or not piece_right or (piece_left.locked and not piece_left.connect_right()) or (piece_right.locked and not piece_right.connect_left()) or (piece_up and piece_up.locked and piece_up.connect_down()) or (piece_down and piece_down.locked and piece_down.connect_up()):
				return [Piece(LV)]
			actions = []
			if (piece_up.connect_down() or not piece_up.locked) or (piece_down.connect_up() or not piece_down.locked):
				actions.append(Piece(LV))
			if (piece_left.connect_right() or not piece_left.locked) or (piece_right.connect_left() or not piece_right.locked):
				actions.append(Piece(LH))
			return actions

	def queue_values(self, row :int, col :int):
		""" Devolve as coordenadas das peças adjacentes à peça na posição (row, col) que ainda não estão na queue."""
		res = []
		up, down = self.adjacent_vertical_values(row, col)
		left, right = self.adjacent_horizontal_values(row, col)
		if up and not up.locked and up:
			res.append((row -1, col))
		if down and not down.locked and down:
			res.append((row +1 , col))
		if left and not left.locked and left:
			res.append((row, col - 1))
		if right and not right.locked and right:
			res.append((row, col + 1))
		return res

	def apply_single_actions(self, init_row :int, init_col :int):
		"""	Aplica ações a peças que só têm uma ação que faça sentido pelo objetivo do jogo, começando pelas peças
			adjacentes à peça em (init_row, init_col) e expandindo para o resto do tabuleiro."""
		Next_plays = []
		if init_row and init_col:
			Next_plays.extend(self.queue_values(init_row, init_col))
	
		for i in range(self.size):
			possible_values = self.get_actions(0, i)
			if len(possible_values) == 1:
				old_piece_disconnections = self.get_adjacent_disconnections(0, i)
				self.set_value(0, i, possible_values[0])
				self.get_value(0, i).lock()
				Next_plays.extend(self.queue_values(0, i))
				self.disconnections -= old_piece_disconnections
				self.disconnections += self.get_adjacent_disconnections(0, i)
			possible_values = self.get_actions(self.size - 1, i)
			if len(possible_values) == 1:
				old_piece_disconnections = self.get_adjacent_disconnections(self.size - 1, i)
				self.set_value(self.size - 1, i, possible_values[0])
				self.get_value(self.size - 1, i).lock()
				Next_plays.extend(self.queue_values(self.size - 1, i))
				self.disconnections -= old_piece_disconnections
				self.disconnections += self.get_adjacent_disconnections(self.size - 1, i)
			possible_values = self.get_actions(i, 0)
			if len(possible_values) == 1:
				old_piece_disconnections = self.get_adjacent_disconnections(i, 0)
				self.set_value(i, 0, possible_values[0])
				self.get_value(i, 0).lock()
				Next_plays.extend(self.queue_values(i, 0))
				self.disconnections -= old_piece_disconnections
				self.disconnections += self.get_adjacent_disconnections(i, 0)
			possible_values = self.get_actions(i, self.size - 1)
			if len(possible_values) == 1:
				old_piece_disconnections = self.get_adjacent_disconnections(i, self.size - 1)
				self.set_value(i, self.size - 1, possible_values[0])
				self.get_value(i, self.size - 1).lock()
				Next_plays.extend(self.queue_values(i, self.size -1))
				self.disconnections -= old_piece_disconnections
				self.disconnections += self.get_adjacent_disconnections(i, self.size - 1)

		# Center
		while Next_plays:
			row, col = Next_plays.pop(0)
			possible_values = self.get_actions(row, col)
			if len(possible_values) == 1:
				old_piece_disconnections = self.get_adjacent_disconnections(row, col)
				self.set_value(row, col, possible_values[0])
				self.get_value(row, col).lock()
				Next_plays.extend(self.queue_values(row, col))
				self.disconnections -= old_piece_disconnections
				self.disconnections += self.get_adjacent_disconnections(row, col)

	def has_clusters(self) -> bool:
		"""	Função que verifica se há clusters num tabuleiro totalmente connectado. """
		counter = 0
		stack = [(0, 0)]
		visited = np.zeros((self.size, self.size))
		while stack:
			row, col = stack.pop()
			counter += 1
			visited[row][col] = 1
			piece = self.get_value(row, col)
			piece_up, piece_down = self.adjacent_vertical_values(row, col)
			piece_left, piece_right = self.adjacent_horizontal_values(row, col)
			if piece_up and piece.connect_up() and piece_up.connect_down() and (row - 1, col) not in stack and not visited[row - 1][col]:
				stack.append((row - 1, col))
			if piece_down and piece.connect_down() and piece_down.connect_up() and (row + 1, col) not in stack and not visited[row + 1][col]:
				stack.append((row + 1, col))
			if piece_left and piece.connect_left() and piece_left.connect_right() and (row, col - 1) not in stack and not visited[row][col - 1]:
				stack.append((row, col - 1))
			if piece_right and piece.connect_right() and piece_right.connect_left() and (row, col + 1) not in stack and not visited[row][col + 1]:
				stack.append((row, col + 1))
		return counter != self.size**2

	@staticmethod
	def parse_instance():
		""" Lê a instância do problema do standard input e devolve um objeto da classe Board."""
		first_line = input().split("\t")
		size = len(first_line)
		matrix = np.empty((size, size), dtype=object)
		matrix[0] = [Piece.parse_piece(piece) for piece in first_line]
		for i in range(1, size):
			matrix[i] = [Piece.parse_piece(piece) for piece in input().split("\t")]
		return Board(matrix, size)

	def __str__(self):
		"""	Converte o tabuleiro para uma string no formato pedido."""
		return "\n".join(["\t".join([str(self.matrix[row][col]) for col in range(self.size)]) for row in range(self.size)])

	def deepcopy(self):
		"""	Devolve uma cópia do tabuleiro."""
		return Board(self.matrix.copy(), self.size)

	def pretty_print(self):
		"""	Imprime o tabuleiro num formato mais legível. Ajuda a debuggar."""
		for row in range(self.size):
			row_string_1 = ""
			row_string_2 = ""
			row_string_3 = ""
			for col in range(self.size):
				pretty_strings = self.get_value(row, col).pretty_str()
				row_string_1 += pretty_strings[0]
				row_string_2 += pretty_strings[1]
				row_string_3 += pretty_strings[2]
			print(row_string_1)
			print(row_string_2)
			print(row_string_3)	

	def lock_print(self):
		"""	Imprime o tabuleiro com 1s nas posições bloqueadas e 0s nas posições desbloqueadas."""
		print("\n")
		for row in range(self.size):
			for col in range(self.size):
				if (self.get_value(row, col).locked):
					print("1\t", end = "")
				else:
					print("0\t", end = "")	
			print()

class PipeManiaState:
	"""	Representação interna de um estado do jogo PipeMania."""
	state_id = 0

	def __init__(self, board: Board, play: "tuple[int, int]"):
		self.board = board.deepcopy()
		self.play = play
		self.id = PipeManiaState.state_id
		PipeManiaState.state_id += 1

	def __lt__(self, other):
		return self.id < other.id
	
	def next_play(self) -> "tuple[int, int]":
		"""	Retorna a próxima peça a rodar: a próxima peça no tabuleiro que não está bloqueada. Retorna a mesma peça
			caso todas as peças estejam bloqueadas."""
		current = self.play
		counter = 0
		next = self.board.get_next(current)
		counter += 1
		while self.board.get_value(next[0], next[1]).locked and counter < self.board.size**2:
			next = self.board.get_next(next)
			counter += 1
		return next

class PipeMania(Problem):
	def __init__(self, board: Board):
		self.initial = PipeManiaState(board, (0, 0))
		self.initial.play = self.initial.next_play()

	def actions(self, state: PipeManiaState) -> "list[Piece]":
		"""Retorna uma lista de ações que podem ser executadas a partir do estado passado como argumento."""
		return state.board.get_actions(state.play[0], state.play[1])

	def result(self, state: PipeManiaState, action: Piece):
		"""	Retorna o estado resultante de executar a 'action' sobre 'state' passado como argumento. A ação a executar
			deve ser uma das presentes na lista obtida pela execução de self.actions(state)."""

		# Aplica a ação ao estado
		new_state = PipeManiaState(state.board, state.play)
		new_state.board.set_value(state.play[0], state.play[1], action)
		new_state.board.get_value(state.play[0], state.play[1]).lock()

		# Atualizar o número de desconexões
		piece_original_disconnections = state.board.get_adjacent_disconnections(state.play[0], state.play[1])
		piece_new_disconnections = new_state.board.get_adjacent_disconnections(state.play[0], state.play[1])
		new_state.board.disconnections = state.board.disconnections - piece_original_disconnections + piece_new_disconnections

		# Aplica ações únicas
		new_state.board.apply_single_actions(state.play[0], state.play[1])

		# Atualiza a próxima jogada
		new_state.play = new_state.next_play()

		return new_state

	def goal_test(self, state: PipeManiaState):
		"""	Determina se o estado atual é um estado objetivo. O estado é objetivo se não houver desconexões e não houver
			clusters."""
		
		return state.board.disconnections == 0 and not state.board.has_clusters()

	def h(self, node: Node):
		"""	Função heurística que estima o custo de chegar a um estado objetivo a partir do estado passado como
			argumento. Não foi utilizado neste projeto."""
		return node.state.board.disconnections


profiler = cProfile.Profile()
profiler.enable()

# Lê a instância do problema
board = Board.parse_instance()

# Inicialmente, roda todas as peças que só têm uma ação possível
board.apply_single_actions(None, None)

# Cria o problema
problem = PipeMania(board)

# Executa a procura em profundidade
goal_node = depth_first_tree_search(problem)

# Imprime a solução
print(str(goal_node.state.board))

profiler.disable()
profiler.print_stats()