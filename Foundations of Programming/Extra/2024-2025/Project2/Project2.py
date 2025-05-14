# FP 2024
# Enzo Nunes 106336
# Orbito-n

########################################################################################################################
#                                                     Type Aliases                                                     #
########################################################################################################################

type Orbit = int
type Position = tuple[str, int]
type Stone = str
type Board = list[list[Stone]]
type Line = tuple[tuple[Position, Stone], ...]

########################################################################################################################
#                                                   Auxiliar Functions                                                 #
########################################################################################################################


def character_offset(character: str, n: int) -> str:
    """Receives a character and an integer n and returns the character that is n positions ahead in the alphabet."""

    return chr(ord(character) + n)


def character_index(character: str) -> int:
    """Receives a string and returns the index of the character in the alphabet."""

    return ord(character) - ord("a")


def eh_orbita(arg) -> bool:
    """Receives any arg and determines if it's a valid orbit number."""

    return isinstance(arg, int) and 2 <= arg <= 5


########################################################################################################################
#                                               2.1 Abstract Data Types                                                #
########################################################################################################################

#####################
# 2.1.1 TAD posicao #
#####################


# Constructors
def cria_posicao(column: str, row: int) -> Position:
    """Receives a column and a row and returns a position type."""

    if not (
        isinstance(column, str)
        and isinstance(row, int)
        and column.islower()
        and len(column) == 1
        and 1 <= row <= 10
        and "a" <= column <= character_offset("a", 9)
    ):
        raise ValueError("cria_posicao: argumentos invalidos")

    return (column, row)


# Getters
def obtem_pos_col(position: Position) -> str:
    """Receives a position and returns its column."""

    return position[0]


def obtem_pos_lin(position: Position) -> int:
    """Receives a position and returns its row."""

    return position[1]


# Type Checkers
def eh_posicao(arg) -> bool:
    """Receives any arg and determines if it's a valid position."""

    return (
        isinstance(arg, tuple)
        and len(arg) == 2
        and isinstance(arg[1], int)
        and isinstance(arg[0], str)
        and 1 <= arg[1] <= 10
        and arg[0].islower()
    )


# Testers
def posicoes_iguais(position1: Position, position2: Position) -> bool:
    """Receives two positions and determines if they are equal."""

    return position1 == position2


# Transformers
def posicao_para_str(position: Position) -> str:
    """Receives a position and returns the string that represents it in the specified format."""

    return f"{position[0]}{position[1]}"


def str_para_posicao(string: str) -> Position:
    """Receives a string and returns the position it represents."""

    return (string[0], int(string[1:]))


# High Order Functions
def eh_posicao_valida(position: Position, n: Orbit) -> bool:
    """Receives a position and a board's number of orbits and determines if the position is valid in that board."""

    return (
        eh_posicao(position)
        and eh_orbita(n)
        and 1 <= obtem_pos_lin(position) <= 2 * n
        and "a" <= obtem_pos_col(position) <= character_offset("a", 2 * n - 1)
    )


def obtem_posicoes_adjacentes(position: Position, n: Orbit, include_diagonals: bool) -> tuple[Position, ...]:
    """Receives a position, a board's number of orbits and a boolean and returns a tuple with the adjacent positions."""

    column, row = obtem_pos_col(position), obtem_pos_lin(position)

    return tuple(
        adjacent_position
        for row_offset, column_offset in (
            (-1, 0),
            (-1, 1),
            (0, 1),
            (1, 1),
            (1, 0),
            (1, -1),
            (0, -1),
            (-1, -1),
        )
        if (
            (include_diagonals and (column_offset != 0 or row_offset != 0))
            or (not include_diagonals and abs(column_offset) != abs(row_offset))
        )
        and not ((row == 1 and row_offset == -1) or (column == "a" and column_offset == -1))
        and eh_posicao_valida(
            adjacent_position := cria_posicao(character_offset(column, column_offset), row + row_offset),
            n,
        )
    )


def obtem_numero_orbita_posicao(position: Position, n: Orbit) -> Orbit:
    """Receives a position and a board's number of orbits and returns the orbit it belongs to."""

    board_length = 2 * n
    top_border_distance = obtem_pos_lin(position) - 1
    bottom_border_distance = board_length - top_border_distance - 1
    left_border_distance = character_index(obtem_pos_col(position))
    right_border_distance = board_length - left_border_distance - 1

    return n - min(top_border_distance, bottom_border_distance, left_border_distance, right_border_distance)


def ordena_posicoes(positions: tuple[Position, ...], n: int) -> tuple[Position, ...]:
    """Receives a tuple of positions and returns it sorted in ascending order."""

    return tuple(
        sorted(
            positions,
            key=lambda position: (
                obtem_numero_orbita_posicao(position, n),
                obtem_pos_lin(position),
                obtem_pos_col(position),
            ),
        ),
    )


###################
# 2.1.2 TAD pedra #
###################


# Constructors
def cria_pedra_branca() -> Stone:
    """Returns a white stone."""

    return "O"


def cria_pedra_preta() -> Stone:
    """Returns a black stone."""

    return "X"


def cria_pedra_neutra() -> Stone:
    """Returns a neutral stone."""

    return " "


# Type-Checkers
def eh_pedra(arg) -> bool:
    """Receives any arg and determines if it's a valid stone."""

    return arg in ("O", "X", " ")


def eh_pedra_branca(stone: Stone) -> bool:
    """Receives a stone and determines if it's white."""

    return stone == "O"


def eh_pedra_preta(stone: Stone) -> bool:
    """Receives a stone and determines if it's black."""

    return stone == "X"


# Testers
def pedras_iguais(stone1: Stone, stone2: Stone) -> bool:
    """Receives two stones and determines if they are equal."""

    return stone1 == stone2


# Transformers
def pedra_para_str(stone: Stone) -> str:
    """Receives a stone and returns the string that represents it in the specified format."""

    return stone


# High Order Functions
def eh_pedra_jogador(stone: Stone) -> bool:
    """Receives a stone and determines if it belongs to a player."""

    return eh_pedra_branca(stone) or eh_pedra_preta(stone)


def pedra_para_int(stone: Stone) -> int:
    """Receives a stone and returns the integer that represents it."""

    return {cria_pedra_branca(): -1, cria_pedra_preta(): 1, cria_pedra_neutra(): 0}[stone]


#######################
# 2.1.3 TAD tabuleiro #
#######################


# Constructors
def cria_tabuleiro_vazio(n: Orbit) -> Board:
    """Receives an integer and returns an empty board with n orbits."""

    if not eh_orbita(n):
        raise ValueError("cria_tabuleiro_vazio: argumento invalido")

    return [[cria_pedra_neutra() for _ in range(2 * n)] for _ in range(2 * n)]


def cria_tabuleiro(n: Orbit, black_stones: tuple[Position, ...], white_stones: tuple[Position, ...]) -> Board:
    """Receives an integer and two tuples of positions and returns a board with n orbits and the specified stones."""

    if not (
        eh_orbita(n)
        and isinstance(black_stones, tuple)
        and isinstance(white_stones, tuple)
        and all(eh_posicao_valida(position, n) for position in black_stones)
        and all(eh_posicao_valida(position, n) for position in white_stones)
    ):
        raise ValueError("cria_tabuleiro: argumentos invalidos")

    board = cria_tabuleiro_vazio(n)

    for position, stone in zip(
        black_stones + white_stones,
        [cria_pedra_preta()] * len(black_stones) + [cria_pedra_branca()] * len(white_stones),
    ):
        board[obtem_pos_lin(position) - 1][character_index(obtem_pos_col(position))] = stone

    return board


def cria_copia_tabuleiro(board: Board) -> Board:
    """Receives a board and returns a deep copy of it."""

    return [[stone for stone in row] for row in board]


# Getters
def obtem_numero_orbitas(board: Board) -> Orbit:
    """Receives a board and returns the number of orbits it has."""

    return len(board) // 2


def obtem_pedra(board: Board, position: Position) -> Stone:
    """Receives a board and a position and returns the stone in that position."""

    return board[obtem_pos_lin(position) - 1][character_index(obtem_pos_col(position))]


def obtem_linha_horizontal(board: Board, position: Position) -> Line:
    """Receives a board and a position and returns the row that contains that position."""

    row = obtem_pos_lin(position)

    return tuple(
        (
            cria_posicao(character_offset("a", column_offset), row),
            board[row - 1][column_offset],
        )
        for column_offset in range(len(board))
    )


def obtem_linha_vertical(board: Board, position: Position) -> Line:
    """Receives a board and a position and returns the column that contains that position."""

    column = obtem_pos_col(position)

    return tuple(
        (
            cria_posicao(column, row + 1),
            board[row][character_index(column)],
        )
        for row in range(len(board))
    )


def obtem_linhas_diagonais(board: Board, position: Position) -> tuple[Line, Line]:
    """Receives a board and a position and returns the two diagonals that contain that position."""

    column, row = obtem_pos_col(position), obtem_pos_lin(position)

    return (
        tuple(
            (
                cria_posicao(character_offset(column, offset), row + offset),
                board[row + offset - 1][character_index(character_offset(column, offset))],
            )
            for offset in range(
                max(1 - row, -character_index(column)),
                min(len(board) - row + 1, len(board) - character_index(column)),
            )
        ),
        tuple(
            (
                cria_posicao(character_offset(column, offset), row - offset),
                board[row - offset - 1][character_index(character_offset(column, offset))],
            )
            for offset in range(
                max(-character_index(column), row - len(board)),
                min(len(board) - character_index(column), row),
            )
        ),
    )


def obtem_posicoes_pedra(board: Board, stone: Stone) -> tuple[Position, ...]:
    """Receives a board and a stone and returns the positions in which that stone is in the specified order."""

    return ordena_posicoes(
        tuple(
            cria_posicao(character_offset("a", column_index), row_index + 1)
            for row_index, row in enumerate(board)
            for column_index, current_stone in enumerate(row)
            if current_stone == stone
        ),
        obtem_numero_orbitas(board),
    )


# Setters
def coloca_pedra(board: Board, position: Position, stone: Stone) -> Board:
    """Receives a board, a stone and a position and places the stone in that position."""

    board[obtem_pos_lin(position) - 1][character_index(obtem_pos_col(position))] = stone
    return board


def remove_pedra(board: Board, position: Position) -> Board:
    """Receives a board and a position and removes the stone in that position."""

    return coloca_pedra(board, position, cria_pedra_neutra())


# Recognizers
def eh_tabuleiro(arg) -> bool:
    """Receives any arg and determines if it's a valid board."""

    return (
        isinstance(arg, list)
        and len(arg) in (4, 6, 8, 10)
        and all(isinstance(row, list) and len(row) == len(arg) and all(eh_pedra(pedra) for pedra in row) for row in arg)
    )


# Testers
def tabuleiros_iguais(board1: Board, board2: Board) -> bool:
    """Receives two boards and determines if they are equal."""

    return obtem_numero_orbitas(board1) == obtem_numero_orbitas(board2) and all(
        pedras_iguais(stone1, stone2) for row1, row2 in zip(board1, board2) for stone1, stone2 in zip(row1, row2)
    )


# Transformers
def tabuleiro_para_str(board: Board) -> str:
    """Receives a board and returns the string that represents it in the specified format."""

    board_length = len(board)
    return "\n".join(
        (
            f"    {"   ".join(character_offset('a', j)
                              for j in range(board_length))}"
            if i == 1
            else (
                (f"0{i // 2}" if i < 20 else "10")
                + f" [{"]-[".join(obtem_pedra(board, cria_posicao(character_offset("a", j), i // 2))
                                  for j in range(board_length))}]"
                if i % 2 == 0
                else f"    {"   ".join("|" for _ in range(board_length))}"
            )
        )
        for i in range(1, 2 * board_length + 1)
    )


# High Order Functions
def move_pedra(board: Board, position1: Position, position2: Position) -> Board:
    """Receives a board and two positions and moves the stone from the first position to the second."""
    return coloca_pedra(remove_pedra(board, position1), position2, obtem_pedra(board, position1))


def obtem_posicao_seguinte(board: Board, position: Position, clockwise: bool) -> Position:
    """Receives a board, a position and a boolean and returns the next position in the specified direction."""
    board_orbit_number = obtem_numero_orbitas(board)
    position_orbit_number = obtem_numero_orbita_posicao(position, board_orbit_number)
    row, column = obtem_pos_lin(position), obtem_pos_col(position)

    adjacent_orbit_positions = ordena_posicoes(
        tuple(
            filter(
                lambda adjacent_position: obtem_numero_orbita_posicao(adjacent_position, board_orbit_number)
                == position_orbit_number,
                obtem_posicoes_adjacentes(position, board_orbit_number, False),
            )
        ),
        board_orbit_number,
    )

    return adjacent_orbit_positions[
        clockwise != (row <= board_orbit_number)
        if row - 1 == character_index(column)
        else clockwise != (row - 1 >= character_index(column))
    ]


def roda_tabuleiro(board: Board) -> Board:
    """Receives a board and rotates all it's stones one position anti-clockwise."""

    old_board = cria_copia_tabuleiro(board)
    board_length = 2 * obtem_numero_orbitas(board)

    for i in range(board_length):
        for j in range(board_length):
            position = cria_posicao(character_offset("a", j), i + 1)
            coloca_pedra(
                board,
                obtem_posicao_seguinte(
                    board,
                    position,
                    False,
                ),
                obtem_pedra(old_board, position),
            )

    return board


def verifica_linha_pedras(board: Board, position: Position, stone: Stone, k: int) -> bool:
    """Receives a board, a position, a stone and an integer and determines if there are k stones in a row."""

    def obtem_linha_jogador(line: Line) -> int:
        """Receives a line and returns the number of stones in a row that belong to the player."""
        counter = 1
        index = line.index((position, obtem_pedra(board, position))) - 1
        while index >= 0 and pedras_iguais(line[index][1], stone):
            counter += 1
            index -= 1

        index = line.index((position, obtem_pedra(board, position))) + 1
        while index < len(line) and pedras_iguais(line[index][1], stone):
            counter += 1
            index += 1

        return counter

    return pedras_iguais(stone, obtem_pedra(board, position)) and any(
        obtem_linha_jogador(line) >= k
        for line in (
            (
                obtem_linha_horizontal(board, position),
                obtem_linha_vertical(board, position),
            )
            + obtem_linhas_diagonais(board, position)
        )
    )


########################################################################################################################
#                                               2.2 Additional Functions                                               #
########################################################################################################################


# 2.2.1
def eh_vencedor(board: Board, stone: Stone) -> bool:
    """Receives a board and a stone and determines if the player that owns that stone is the winner."""

    return any(
        verifica_linha_pedras(board, position, stone, 2 * obtem_numero_orbitas(board))
        for position in obtem_posicoes_pedra(board, stone)
    )


# 2.2.2
def eh_fim_jogo(board: Board) -> bool:
    """Receives a board and determines if the game is over."""

    return any(
        eh_vencedor(board, stone) for stone in (cria_pedra_branca(), cria_pedra_preta())
    ) or not obtem_posicoes_pedra(board, cria_pedra_neutra())


# 2.2.3
def escolhe_movimento_manual(board: Board) -> Position:
    """Receives a board and returns a move chosen by the player."""

    position_string = input("Escolha uma posicao livre:")
    while not (
        len(position_string) > 1
        and position_string[1:].isnumeric()
        and position_string[0].isalpha()
        and position_string[0].islower()
        and eh_posicao_valida(position := str_para_posicao(position_string), obtem_numero_orbitas(board))
        and obtem_pedra(board, position) == cria_pedra_neutra()
    ):
        position_string = input("Escolha uma posicao livre:")

    return position


# 2.2.4
def escolhe_movimento_auto(board: Board, stone: Stone, difficulty: str) -> Position:
    """Receives a board, a stone and a difficulty level and returns a move chosen by the computer."""

    ordered_free_positions = obtem_posicoes_pedra(board, cria_pedra_neutra())

    def escolhe_movimento_auto_facil() -> Position:
        """Returns the position chosen by the computer in easy mode."""

        return next(
            (
                position
                for position in ordered_free_positions
                if any(
                    pedras_iguais(
                        stone,
                        obtem_pedra(
                            roda_tabuleiro(cria_copia_tabuleiro(board)),
                            adjacent_position,
                        ),
                    )
                    for adjacent_position in obtem_posicoes_adjacentes(
                        obtem_posicao_seguinte(board, position, False),
                        obtem_numero_orbitas(board),
                        True,
                    )
                )
            ),
            ordered_free_positions[0],
        )

    def escolhe_movimento_auto_normal() -> Position:
        """Returns the position chosen by the computer in normal mode."""

        opponent_stone = cria_pedra_preta() if eh_pedra_branca(stone) else cria_pedra_branca()

        for line_length in range(2 * obtem_numero_orbitas(board), 0, -1):
            winning = tuple(
                position
                for position in ordered_free_positions
                if verifica_linha_pedras(
                    roda_tabuleiro(coloca_pedra(cria_copia_tabuleiro(board), position, stone)),
                    obtem_posicao_seguinte(board, position, False),
                    stone,
                    line_length,
                )
            )

            blocking = tuple(
                position
                for position in ordered_free_positions
                if verifica_linha_pedras(
                    roda_tabuleiro(
                        coloca_pedra(
                            roda_tabuleiro(cria_copia_tabuleiro(board)),
                            obtem_posicao_seguinte(board, position, False),
                            opponent_stone,
                        )
                    ),
                    obtem_posicao_seguinte(board, obtem_posicao_seguinte(board, position, False), False),
                    opponent_stone,
                    line_length,
                )
            )

            if winning:
                return winning[0]
            if blocking:
                return blocking[0]
        return ordered_free_positions[0]

    match difficulty:
        case "facil":
            return escolhe_movimento_auto_facil()
        case "normal":
            return escolhe_movimento_auto_normal()
        case _:
            raise ValueError(f"Invalid difficulty level: {difficulty}")


# 2.2.5
def orbito(n: Orbit, mode: str, player_str: str) -> int:
    """Receives the number of orbits, a game mode and the player's stone and allows the player to play a game of orbito."""

    if not (eh_orbita(n) and mode in ("facil", "normal", "2jogadores") and player_str in ("O", "X")):
        raise ValueError("orbito: argumentos invalidos")

    print(f"Bem-vindo ao ORBITO-{n}.")
    if mode == "2jogadores":
        print("Jogo para dois jogadores.")
    else:
        print(f"Jogo contra o computador ({mode}).")
        print(f"O jogador joga com '{player_str}'.")

    board = cria_tabuleiro_vazio(n)
    print(tabuleiro_para_str(board))

    white_stone = cria_pedra_branca()
    black_stone = cria_pedra_preta()
    player_stone = white_stone if player_str == "O" else black_stone
    machine_stone = white_stone if player_stone == black_stone else black_stone

    def troca_jogador(player: Stone) -> Stone:
        """Returns the other player's stone."""
        return white_stone if player == black_stone else black_stone

    current_player = black_stone
    while not eh_fim_jogo(board):
        if mode == "2jogadores":
            print(f"Turno do jogador '{current_player}'.")
            coloca_pedra(board, escolhe_movimento_manual(board), current_player)
            current_player = troca_jogador(current_player)
        else:
            if current_player == player_stone:
                print("Turno do jogador.")
                coloca_pedra(board, escolhe_movimento_manual(board), player_stone)
                current_player = troca_jogador(current_player)
            else:
                print(f"Turno do computador ({mode}):")
                coloca_pedra(board, escolhe_movimento_auto(board, machine_stone, mode), machine_stone)
                current_player = troca_jogador(current_player)
        roda_tabuleiro(board)
        print(tabuleiro_para_str(board))

    if mode == "2jogadores":
        if eh_vencedor(board, white_stone):
            if eh_vencedor(board, black_stone):
                print("EMPATE")
                return 0
            print("VITORIA DO JOGADOR 'O'")
            return -1
        elif eh_vencedor(board, black_stone):
            print("VITORIA DO JOGADOR 'X'")
            return 1
    else:
        if eh_vencedor(board, player_stone):
            if eh_vencedor(board, machine_stone):
                print("EMPATE")
                return 0
            print("VITORIA")
            return pedra_para_int(player_stone)
        print("DERROTA")
        return pedra_para_int(machine_stone)
    print("EMPATE")
    return 0
