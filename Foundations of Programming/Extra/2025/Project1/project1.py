# FP 2025
# Enzo Nunes 106336


ALPHABET = "ABCÇDEFGHIJLMNOPQRSTUVXZ"
BOARD_SIZE = 15

type Position = tuple[int, int]
type Board = list[list[str]]
type Sack = dict[str, int]
type Player = dict[str, any]


def is_quantity(arg: any) -> bool:
    return type(arg) is int and arg >= 1


def is_uppercase_letter(arg: any) -> bool:
    return type(arg) is str and len(arg) == 1 and arg in ALPHABET


def cria_conjunto(letters: tuple[str], quantities: tuple[int]) -> Sack:
    if not (
        type(letters) is tuple
        and type(quantities) is tuple
        and len(letters) == len(quantities) == len(set(letters))
        and all(is_quantity(quantity) for quantity in quantities)
        and all(is_uppercase_letter(letter) for letter in letters)
    ):
        raise ValueError("cria_conjunto: argumentos inválidos")

    return {letter: quantity for letter, quantity in zip(letters, quantities)}


def gera_numero_aleatorio(seed: int) -> int:
    seed ^= (seed << 13) & 0xFFFFFFFF
    seed ^= (seed >> 17) & 0xFFFFFFFF
    seed ^= (seed << 5) & 0xFFFFFFFF
    return seed


def permuta_letras(letters: list[str], seed: int) -> None:
    for i in range(len(letters) - 1, 0, -1):
        seed = gera_numero_aleatorio(seed)
        j = seed % (i + 1)
        letters[i], letters[j] = letters[j], letters[i]


def baralha_conjunto(sack: Sack, seed: int) -> list[str]:
    letters = []
    for letter, quantity in sorted(sack.items(), key=lambda item: ALPHABET.index(item[0])):
        letters.extend([letter] * quantity)
    permuta_letras(letters, seed)
    return letters


def testa_palavra_padrao(word: str, pattern: str, sack: Sack) -> bool:
    if len(word) != len(pattern):
        return False

    remaining_letters = sack.copy()
    for word_character, pattern_character in zip(word, pattern):
        if word_character != pattern_character:
            if pattern_character != "." or remaining_letters.get(word_character, 0) == 0:
                return False
            remaining_letters[word_character] -= 1

    return True


def cria_tabuleiro() -> list[list[str]]:
    return [["." for _ in range(BOARD_SIZE)] for _ in range(BOARD_SIZE)]


def cria_casa(row: int, column: int) -> Position:
    if not (type(row) is int and type(column) is int and (1 <= row <= BOARD_SIZE) and (1 <= column <= BOARD_SIZE)):
        raise ValueError("cria_casa: argumentos inválidos")
    return (row, column)


def obtem_valor(board: Board, position: Position) -> str:
    return board[position[0] - 1][position[1] - 1]


def insere_letra(board: Board, position: Position, letter: str) -> Board:
    board[position[0] - 1][position[1] - 1] = letter
    return board


def obtem_sequencia(board: Board, position: Position, direction: str, length: int) -> str:
    row, column = map(lambda x: x - 1, position)
    match direction:
        case "H":
            return "".join(
                obtem_valor(board, (row + 1, column + i + 1)) for i in range(length) if 0 <= column + i < BOARD_SIZE
            )
        case "V":
            return "".join(
                obtem_valor(board, (row + i + 1, column + 1)) for i in range(length) if 0 <= row + i < BOARD_SIZE
            )


def insere_palavra(board: Board, position: Position, direction: str, word: str) -> Board:
    row, column = map(lambda x: x - 1, position)
    match direction:
        case "H":
            for i, letter in enumerate(word):
                insere_letra(board, (row + 1, column + i + 1), letter)
        case "V":
            for i, letter in enumerate(word):
                insere_letra(board, (row + i + 1, column + 1), letter)
    return board


def tabuleiro_para_str(board: Board) -> str:
    return "\n".join(
        [
            "                       1 1 1 1 1 1",
            "     1 2 3 4 5 6 7 8 9 0 1 2 3 4 5",
            "   +-------------------------------+",
            *(f"{i:2d} | {' '.join(row)} |" for i, row in enumerate(board, start=1)),
            "   +-------------------------------+",
        ]
    )


def cria_jogador(id: int, score: int, sack: Sack) -> Player:
    if not (
        type(id) is int
        and 1 <= id <= 4
        and type(score) is int
        and score >= 0
        and type(sack) is dict
        and all((is_uppercase_letter(letter) and is_quantity(quantity)) for letter, quantity in sack.items())
        and sum(sack.values()) <= 7
    ):
        raise ValueError("cria_jogador: argumentos inválidos")

    return {"id": id, "pontos": score, "letras": sack}


def jogador_para_str(player: Player) -> str:
    return (
        f"#{player['id']} "
        + f"({player['pontos']:3d}): "
        + " ".join(
            " ".join(letter * player["letras"][letter])
            for letter in filter(
                lambda letter: player["letras"][letter] > 0, sorted(player["letras"].keys(), key=ALPHABET.index)
            )
        )
    )


def distribui_letra(letters: list[str], player: Player) -> bool:
    if not letters:
        return False

    letter = letters.pop()
    player["letras"][letter] = player["letras"].get(letter, 0) + 1
    return True


def play_includes_center(position: Position, direction: str, length: int) -> bool:
    row, column = map(lambda x: x - 1, position)
    match direction:
        case "H":
            return row == 7 and (7 in range(column, column + length))
        case "V":
            return column == 7 and (7 in range(row, row + length))


def is_board_empty(board: Board) -> bool:
    return all(letter == "." for row in board for letter in row)


def play_in_board(position: Position, direction: str, length: int) -> bool:
    row, column = map(lambda x: x - 1, position)
    match direction:
        case "H":
            return 0 <= row < BOARD_SIZE and 0 <= column < BOARD_SIZE and column + length <= BOARD_SIZE
        case "V":
            return 0 <= row < BOARD_SIZE and 0 <= column < BOARD_SIZE and row + length <= BOARD_SIZE


def is_empty_sequence(sequence: str) -> bool:
    return all(letter == "." for letter in sequence)


def joga_palavra(
    board: Board, word: str, position: Position, direction: str, sack: Sack, first_play: bool
) -> tuple[str]:
    sequence = obtem_sequencia(board, position, direction, len(word))
    if not (
        play_in_board(position, direction, len(word))
        and (
            (first_play and is_board_empty(board) and play_includes_center(position, direction, len(word)))
            or (not first_play and not is_board_empty(board) and not is_empty_sequence(sequence))
        )
        and testa_palavra_padrao(word, sequence, sack)
    ):
        return ()

    insere_palavra(board, position, direction, word)
    placed = sorted(word, key=ALPHABET.index)
    for letter in sequence:
        if letter in placed:
            placed.remove(letter)
    return tuple(placed)


def is_valid_trade(player: Player, sequence: list[str], letters: list[str]) -> bool:
    if len(letters) < 7:
        return False

    for letter in set(sequence):
        if sequence.count(letter) > player["letras"].get(letter, 0):
            return False

    return True


def is_valid_play(row: str, column: str, direction: str, word: str) -> bool:
    return (
        row.isdigit()
        and column.isdigit()
        and (1 <= int(row) <= BOARD_SIZE)
        and (1 <= int(column) <= BOARD_SIZE)
        and direction in ("H", "V")
        and all(is_uppercase_letter(letter) for letter in word)
    )


def processa_jogada(
    board: Board, player: Player, letters: list[str], score_reference: dict[str, int], first_play: bool
) -> bool:
    while True:
        play = input(f"Jogada J{player['id']}: ")
        match play.split():
            case ["P"]:
                return False
            case ["T", *sequence]:
                if not is_valid_trade(player, sequence, letters):
                    continue
                for letter in sequence:
                    player["letras"][letter] -= 1
                    distribui_letra(letters, player)
                return True
            case ["J", row, column, direction, word]:
                if not is_valid_play(row, column, direction, word):
                    continue
                placed = joga_palavra(
                    board, word, cria_casa(int(row), int(column)), direction, player["letras"], first_play
                )
                if not placed:
                    continue
                for letter in word:
                    player["pontos"] += score_reference[letter]
                for letter in placed:
                    player["letras"][letter] -= 1
                    distribui_letra(letters, player)
                return True
            case _:
                continue


def scrabble(num_players: int, sack: Sack, score_reference: dict[str, int], seed: int) -> tuple[int]:
    if not (
        type(num_players) is int
        and 2 <= num_players <= 4
        and type(sack) is dict
        and len(sack) > 0
        and all((is_uppercase_letter(letter) and is_quantity(quantity)) for letter, quantity in sack.items())
        and type(score_reference) is dict
        and len(score_reference) == len(ALPHABET)
        and all((is_uppercase_letter(letter) and is_quantity(score)) for letter, score in score_reference.items())
        and type(seed) is int
    ):
        raise ValueError("scrabble: argumentos inválidos")

    letters = baralha_conjunto(sack, seed)

    board = cria_tabuleiro()
    first_play = True
    passes = [False] * num_players
    players = [cria_jogador(i, 0, {}) for i in range(1, num_players + 1)]
    for i in range(7 * num_players):
        distribui_letra(letters, players[i // 7])

    print("Bem-vindo ao SCRABBLE.")
    current_player = 0
    while not (all(passes) or any((not sum(player["letras"].values()) and not letters) for player in players)):
        print(tabuleiro_para_str(board))
        for player in players:
            print(jogador_para_str(player))
        passes[current_player] = not processa_jogada(
            board, players[current_player], letters, score_reference, first_play
        )
        if not is_board_empty(board):
            first_play = False
        current_player = (current_player + 1) % num_players

    return tuple(player["pontos"] for player in players)


tab = [
    ["C", "P", "H", "T", "I", "J", "E", "A", "M", "U", "X", "R", "G", "S", "N"],
    ["E", "J", "F", "I", "B", "O", "P", "U", "H", "C", "R", "G", "N", "S", "T"],
    ["X", "S", "B", "Z", "V", "P", "D", "H", "R", "O", "Q", "N", "G", "F", "C"],
    ["N", "I", "A", "V", "T", "S", "Z", "R", "O", "Ç", "M", "X", "B", "P", "D"],
    ["T", "F", "G", "N", "L", "Ç", "A", "J", "H", "S", "Q", "U", "V", "B", "X"],
    ["U", "Q", "T", "A", "O", "R", "H", "I", "D", "X", "E", "Ç", "N", "F", "L"],
    ["L", "D", "I", "B", "A", "U", "Z", "M", "P", "C", "X", "R", "V", "Ç", "N"],
    ["U", "P", "V", "E", "Z", "O", "C", "Ç", "B", "R", "M", "I", "X", "H", "L"],
    ["A", "L", "F", "I", "Q", "C", "T", "R", "Ç", "Z", "E", "M", "O", "N", "V"],
    ["G", "I", "M", "B", "T", "Q", "P", "E", "V", "F", "R", "L", "Ç", "H", "A"],
    ["X", "C", "T", "V", "N", "L", "R", "G", "P", "O", "H", "J", "B", "A", "M"],
    ["H", "F", "O", "T", "Q", "L", "J", "I", "D", "S", "Ç", "B", "C", "E", "X"],
    ["O", "G", "D", "V", "X", "C", "H", "L", "I", "F", "U", "Z", "S", "R", "J"],
    ["Z", "E", "Ç", "I", "G", "M", "A", "H", "F", "L", "D", "P", "J", "T", "X"],
    ["O", "T", "V", "X", "B", "Ç", "E", "U", "N", "A", "J", "I", "Q", "G", "S"],
]
