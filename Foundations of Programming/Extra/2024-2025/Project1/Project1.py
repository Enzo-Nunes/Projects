# FP 2024
# Enzo Nunes 106336


###################################
# 2.1 Representation of the board #
###################################


# Extra
def eh_peca(arg) -> bool:
    """Receives any argument and determines if it is a valid piece."""
    return isinstance(arg, int) and arg in (-1, 0, 1)


def eh_peca_jogador(arg) -> bool:
    """Receives any argument and determines if it is a valid player piece"""
    return isinstance(arg, int) and arg in (-1, 1)


# 2.1.1
def eh_tabuleiro(arg) -> bool:
    """Receives any argument and determines if it is a valid mnk board."""
    return (
        isinstance(arg, tuple)
        and 2 <= len(arg) <= 100
        and all(isinstance(element, tuple) for element in arg)
        and all(2 <= len(element) <= 100 for element in arg)
        and all(len(element) == len(arg[0]) for element in arg)
        and all(all(eh_peca(piece) for piece in element) for element in arg)
    )


# 2.1.2
def eh_posicao(arg) -> bool:
    """Receives any argument and determines if it is a valid position."""
    return type(arg) is int and 1 <= arg <= 100 * 100


# 2.1.3
def obtem_dimensao(board: tuple[tuple[int]]) -> tuple[int]:
    """Receives a valid mnk board and returns its dimensions."""
    return (len(board), len(board[0]))


# 2.1.4
def obtem_valor(board: tuple[tuple[int]], position: int) -> int:
    """Receives a valid mnk board and a position and returns the value of the position."""
    line_width = obtem_dimensao(board)[1]
    return board[(position - 1) // line_width][(position - 1) % line_width]


# 2.1.5
def obtem_coluna(board: tuple[tuple[int]], position: int) -> tuple[int]:
    """Receives a valid mnk board and a position and returns a tuple formed by the positions that make up the column of
    that position."""
    column_height, line_width = obtem_dimensao(board)
    column_first = ((position - 1) % line_width) + 1
    return tuple(range(column_first, column_first + ((column_height - 1) * line_width) + 1, line_width))


# 2.1.6
def obtem_linha(board: tuple[tuple[int]], position: int) -> tuple[int]:
    """Receives a valid mnk board and a position and returns a tuple formed by the positions that make up the row of
    that position."""
    line_width = obtem_dimensao(board)[1]
    line_first = (((position - 1) // line_width) * line_width) + 1
    return tuple(range(line_first, line_first + line_width))


# 2.1.7
def obtem_diagonais(board: tuple[tuple[int]], position: int) -> tuple[tuple[int]]:
    """Receives a valid mnk board and a position and returns a tuple formed by the positions that make up the diagonals
    of that position."""

    # Convert position to coordinates
    column_height, line_width = obtem_dimensao(board)
    row = (position - 1) // line_width
    column = (position - 1) % line_width

    # Main Diagonal above the position
    main_diagonal_up = tuple((i * line_width + j + 1) for i, j in zip(range(row, -1, -1), range(column, -1, -1)))

    # Main Diagonal below the position
    main_diagonal_down = tuple(
        (i * line_width + j + 1) for i, j in zip(range(row + 1, column_height), range(column + 1, line_width))
    )

    # Secondary diagonal above the position
    secondary_diagonal_up = tuple(
        (i * line_width + j + 1) for i, j in zip(range(row, -1, -1), range(column, line_width))
    )

    # Secondary diagonal below the position
    secondary_diagonal_down = tuple(
        (i * line_width + j + 1) for i, j in zip(range(row + 1, column_height), range(column - 1, -1, -1))
    )

    # Full diagonals
    main_diagonal = main_diagonal_up[::-1] + main_diagonal_down
    secondary_diagonal = secondary_diagonal_down[::-1] + secondary_diagonal_up

    return tuple(main_diagonal), tuple(secondary_diagonal)


# Extra
def peca_para_str(piece: int) -> str:
    """Receives a piece and returns its string representation."""
    return {-1: "O", 0: "+", 1: "X"}[piece]


# 2.1.8
def tabuleiro_para_str(board: tuple[tuple[int]]) -> str:
    """Receives a valid mnk board and returns a string representation of it."""

    column_height, line_width = obtem_dimensao(board)
    return "\n".join(
        (
            "---".join(
                peca_para_str(obtem_valor(board, j))
                for j in range(((i // 2) * line_width) + 1, ((i // 2) + 1) * line_width + 1)
            )
            if i % 2 == 0
            else "   ".join("|" for _ in range(line_width))
        )
        for i in range(2 * column_height - 1)
    )


################################################
# 2.2 Inspection and manipulation of the board #
################################################


# 2.2.1
def eh_posicao_valida(board: tuple[tuple[int]], position: int) -> bool:
    """Receives a mnk board and a position and determines if the position is valid."""

    if not eh_posicao(position) or not eh_tabuleiro(board):
        raise ValueError("eh_posicao_valida: argumentos invalidos")

    column_height, line_width = obtem_dimensao(board)
    return position <= column_height * line_width


# Extra
def eh_posicao_tipo(board: tuple[tuple[int]], position: int, piece_type: int) -> bool:
    """Receives a mnk board, a position and a piece type and determines if the position is of that type."""
    return obtem_valor(board, position) == piece_type


# 2.2.2
def eh_posicao_livre(board: tuple[tuple[int]], position: int) -> bool:
    """Receives a mnk board and a position and determines if the position is free."""

    if not eh_posicao(position) or not eh_tabuleiro(board) or not eh_posicao_valida(board, position):
        raise ValueError("eh_posicao_livre: argumentos invalidos")

    return eh_posicao_tipo(board, position, 0)


# 2.2.3
def obtem_posicoes_livres(board: tuple[tuple[int]]) -> tuple[int]:
    """Receives a mnk board and returns a tuple with the free positions."""

    if not eh_tabuleiro(board):
        raise ValueError("obtem_posicoes_livres: argumento invalido")

    column_height, line_width = obtem_dimensao(board)
    return tuple(i for i in range(1, column_height * line_width + 1) if eh_posicao_livre(board, i))


# 2.2.4
def obtem_posicoes_jogador(board: tuple[tuple[int]], player: int) -> tuple[int]:
    """Receives a mnk board and a player and returns a tuple with the positions occupied by that player."""

    if not eh_tabuleiro(board) or not eh_peca_jogador(player):
        raise ValueError("obtem_posicoes_jogador: argumentos invalidos")

    column_height, line_width = obtem_dimensao(board)
    return tuple(i for i in range(1, column_height * line_width + 1) if eh_posicao_tipo(board, i, player))


# 2.2.5
def obtem_posicoes_adjacentes(board: tuple[tuple[int]], position: int) -> tuple[int]:
    """Receives a mnk board and a position and returns a tuple with the positions adjacent to that position."""

    if not eh_posicao(position) or not eh_tabuleiro(board) or not eh_posicao_valida(board, position):
        raise ValueError("obtem_posicoes_adjacentes: argumentos invalidos")

    column_height, line_width = obtem_dimensao(board)
    row = (position - 1) // line_width
    column = (position - 1) % line_width

    return tuple(
        ((row + i) * line_width) + column + j + 1
        for i in (-1, 0, 1)
        for j in (-1, 0, 1)
        if 0 <= row + i < column_height
        and 0 <= column + j < line_width
        and ((row + i) * line_width) + column + j + 1 != position
    )


# Extra
def calcula_distancia_centro(board: tuple[tuple[int]], position: int) -> int:
    """Receives a mnk board and a position and returns the distance of the position to the center of the board."""

    column_height, line_width = obtem_dimensao(board)
    center = ((column_height // 2) * line_width) + (line_width // 2) + 1

    return max(
        abs(((center - 1) // line_width) - ((position - 1) // line_width)),
        abs(((center - 1) % line_width) - ((position - 1) % line_width)),
    )


# 2.2.6
def ordena_posicoes_tabuleiro(board: tuple[tuple[int]], positions: tuple[int]) -> tuple[int]:
    """Receives a mnk board and a tuple of positions and returns the tuple sorted in ascending order."""

    if (
        not eh_tabuleiro(board)
        or not isinstance(positions, tuple)
        or not all(eh_posicao(position) and eh_posicao_valida(board, position) for position in positions)
    ):
        raise ValueError("ordena_posicoes_tabuleiro: argumentos invalidos")

    return tuple(sorted(positions, key=lambda position: (calcula_distancia_centro(board, position), position)))


# 2.2.7
def marca_posicao(board: tuple[tuple[int]], position: int, player: int) -> tuple[tuple[int]]:
    """Receives a mnk board, a position and a player and returns a new board with the position marked by the player."""

    if (
        not eh_posicao(position)
        or not eh_tabuleiro(board)
        or not eh_posicao_valida(board, position)
        or not eh_peca(player)
        or not eh_posicao_livre(board, position)
    ):
        raise ValueError("marca_posicao: argumentos invalidos")

    column_height, line_width = obtem_dimensao(board)

    return tuple(
        tuple(
            (
                player
                if i == ((position - 1) // line_width) and j == ((position - 1) % line_width)
                else obtem_valor(board, i * line_width + j + 1)
            )
            for j in range(line_width)
        )
        for i in range(column_height)
    )


# Extra
def eh_k(arg) -> bool:
    """Receives any argument and determines if it is a valid k."""
    return isinstance(arg, int) and 1 <= arg <= 100


# 2.2.8
def verifica_k_linhas(board: tuple[tuple[int]], position: int, player: int, k: int) -> bool:
    """Receives a mnk board, a position, a player and a k and determines if the player has k pieces in a row."""

    def obtem_linhas(board: tuple[tuple[int]], position: int) -> tuple[tuple[int]]:
        """Receives a valid mnk board and a position and returns a tuple formed by the lines that contain the position."""
        return (obtem_linha(board, position), obtem_coluna(board, position)) + obtem_diagonais(board, position)

    def obtem_linha_jogador(board: tuple[tuple[int]], line: tuple[int], position: int, player: int) -> int:
        counter = 1
        index = line.index(position) - 1
        while index >= 0 and obtem_valor(board, line[index]) == player:
            counter += 1
            index -= 1

        index = line.index(position) + 1
        while index < len(line) and obtem_valor(board, line[index]) == player:
            counter += 1
            index += 1

        return counter

    if (
        not eh_posicao(position)
        or not eh_tabuleiro(board)
        or not eh_posicao_valida(board, position)
        or not eh_peca(player)
        or not eh_k(k)
    ):
        raise ValueError("verifica_k_linhas: argumentos invalidos")

    return eh_posicao_tipo(board, position, player) and any(
        obtem_linha_jogador(board, line, position, player) >= k for line in obtem_linhas(board, position)
    )


"""
    ( 1, 0, 0,  1)
    (-1, 1, 0,  1)
    (-1, 0, 0, -1)


"""

################
# 2.3 Gameplay #
################


# 2.3.1
def eh_fim_jogo(board: tuple[tuple[int]], k: int) -> bool:
    """Receives a mnk board and a k and determines if the game is won by any player."""

    if not eh_tabuleiro(board) or not eh_k(k):
        raise ValueError("eh_fim_jogo: argumentos invalidos")

    return any(
        verifica_k_linhas(board, position, player, k)
        for player in (-1, 1)
        for position in obtem_posicoes_jogador(board, player)
    ) or not obtem_posicoes_livres(board)


# 2.3.2
def escolhe_posicao_manual(board: tuple[tuple[int]]) -> int:
    """Receives a mnk board and returns the position chosen by the player."""

    if not eh_tabuleiro(board):
        raise ValueError("escolhe_posicao_manual: argumento invalido")

    position = input("Turno do jogador. Escolha uma posicao livre: ")
    while (
        not position.isnumeric()
        or not eh_posicao(int(position))
        or not eh_posicao_valida(board, int(position))
        or not eh_posicao_livre(board, int(position))
    ):
        position = input("Turno do jogador. Escolha uma posicao livre: ")

    return int(position)


# Extra
def eh_dificuldade(arg) -> bool:
    """Receives any argument and determines if it is a valid difficulty."""
    return isinstance(arg, str) and arg in ("facil", "normal", "dificil")


# Extra
def obtem_vencedor(board: tuple[tuple[int]], k: int) -> int:
    """Receives a mnk board and a k and returns the winner of the game."""
    dimensions = obtem_dimensao(board)
    return next(
        (
            player
            for position in range(1, (dimensions[0] * dimensions[1]) + 1)
            for player in (-1, 1)
            if verifica_k_linhas(board, position, player, k)
        ),
        0,
    )


# 2.3.3
def escolhe_posicao_auto(board: tuple[tuple[int]], player: int, k: int, difficulty: int) -> int:
    """Receives a mnk board, a player, a k and a difficulty and returns the position chosen by the computer."""

    def obtem_posicoes_adjacentes_livres_jogador(board: tuple[tuple[int]], player: int) -> tuple[int]:
        """Receives a mnk board and a player and returns a tuple with the free positions adjacent to the player's
        positions."""
        return tuple(
            set(
                position
                for player_position in obtem_posicoes_jogador(board, player)
                for position in obtem_posicoes_adjacentes(board, player_position)
                if eh_posicao_livre(board, position)
            )
        )

    def escolhe_posicao_auto_facil(board: tuple[tuple[int]], player: int) -> int:
        """Receives a mnk board, a player and a k and returns the position chosen by the computer in easy mode."""

        return (
            ordena_posicoes_tabuleiro(board, adjacent_free_positions)[0]
            if (adjacent_free_positions := obtem_posicoes_adjacentes_livres_jogador(board, player))
            else ordena_posicoes_tabuleiro(board, obtem_posicoes_livres(board))[0]
        )

    def escolhe_posicao_auto_normal(board: tuple[tuple[int]], player: int, k: int) -> int:
        """Receives a mnk board, a player and a k and returns the position chosen by the computer in normal mode."""

        adjacent_free_positions = {
            p: ordena_posicoes_tabuleiro(board, obtem_posicoes_adjacentes_livres_jogador(board, p))
            for p in (player, -player)
        }

        return next(
            (
                position
                for line_length in range(k, 1, -1)
                for possible_player in (player, -player)
                for position in adjacent_free_positions[possible_player]
                if verifica_k_linhas(
                    marca_posicao(board, position, possible_player), position, possible_player, line_length
                )
            ),
            ordena_posicoes_tabuleiro(board, obtem_posicoes_livres(board))[0],
        )

    def escolhe_posicao_auto_dificil(board: tuple[tuple[int]], player: int, k: int) -> int:
        """Receives a mnk board, a player and a k and returns the position chosen by the computer in hard mode."""

        def simula_jogo(board: tuple[tuple[int]], player: int, k: int) -> str:
            """Receives a mnk board, a player and a k and determines if that player would win if both players played
            with the normal strategy."""

            current_player = -player
            current_board = board
            while not eh_fim_jogo(current_board, k):
                current_board = marca_posicao(
                    current_board, escolhe_posicao_auto_normal(current_board, current_player, k), current_player
                )
                current_player = -current_player

            winner = obtem_vencedor(current_board, k)
            return "win" if winner == player else "lose" if winner == -player else "draw"

        adjacent_free_positions = {
            p: ordena_posicoes_tabuleiro(board, obtem_posicoes_adjacentes_livres_jogador(board, p))
            for p in (player, -player)
        }

        # Check for winning move for the player
        winning_move = next(
            (
                position
                for position in adjacent_free_positions[player]
                if verifica_k_linhas(marca_posicao(board, position, player), position, player, k)
            ),
            None,
        )
        if winning_move:
            return winning_move

        # Check for blocking move against the opponent
        blocking_move = next(
            (
                position
                for position in adjacent_free_positions[-player]
                if verifica_k_linhas(marca_posicao(board, position, -player), position, -player, k)
            ),
            None,
        )
        if blocking_move:
            return blocking_move

        # Simulate game results
        results = {"win": (), "draw": (), "lose": ()}
        for position in ordena_posicoes_tabuleiro(board, obtem_posicoes_livres(board)):
            results[simula_jogo(marca_posicao(board, position, player), player, k)] += (position,)

        return results["win"][0] if results["win"] else results["draw"][0] if results["draw"] else results["lose"][0]

    if (
        not eh_tabuleiro(board)
        or not eh_peca(player)
        or not eh_k(k)
        or not eh_dificuldade(difficulty)
        or not obtem_posicoes_livres(board)
        or eh_fim_jogo(board, k)
    ):
        raise ValueError("escolhe_posicao_auto: argumentos invalidos")

    match difficulty:
        case "facil":
            return escolhe_posicao_auto_facil(board, player)
        case "normal":
            return escolhe_posicao_auto_normal(board, player, k)
        case "dificil":
            return escolhe_posicao_auto_dificil(board, player, k)


# 2.3.4
def jogo_mnk(config: tuple[int, int, int], player: int, difficulty: str) -> int:
    """Receives a mnk game configuration, a player and a difficulty and plays the game until either the player or
    computer wins. returns the winner of the game."""

    if (
        not isinstance(config, tuple)
        or len(config) != 3
        or not all(isinstance(arg, int) for arg in config)
        or not all(2 <= arg <= 100 for arg in config)
        or not eh_peca(player)
        or not eh_dificuldade(difficulty)
    ):
        raise ValueError("jogo_mnk: argumentos invalidos")

    m, n, k = config
    machine = -player
    board = tuple(tuple(0 for _ in range(n)) for _ in range(m))

    print("Bem-vindo ao JOGO MNK.")
    print(f"O jogador joga com '{peca_para_str(player)}'.")
    print(tabuleiro_para_str(board))

    current_player = player if player == 1 else machine
    while not eh_fim_jogo(board, k):
        if current_player == player:
            board = marca_posicao(board, escolhe_posicao_manual(board), player)
            current_player = machine
        else:
            print(f"Turno do computador ({difficulty}):")
            board = marca_posicao(board, escolhe_posicao_auto(board, machine, k, difficulty), machine)
            current_player = player
        print(tabuleiro_para_str(board))

    vencedor = obtem_vencedor(board, k)
    if vencedor == player:
        print("VITORIA")
        return player
    elif vencedor == machine:
        print("DERROTA")
        return machine
    else:
        print("EMPATE")
        return 0
