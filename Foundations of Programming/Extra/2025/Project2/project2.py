# FP 2025
# Enzo Nunes 106336


# MARK: Constants


class Direction:
    HORIZONTAL = "H"
    VERTICAL = "V"


class ErrorMessages:
    INVALID_POSITION = "cria_casa: argumentos inválidos"
    INVALID_HUMAN = "cria_humano: argumento inválido"
    INVALID_BOT = "cria_agente: argumento inválido"
    INVALID_VOCABULARY = "cria_vocabulario: argumento inválido"
    INVALID_SCRABBLE = "scrabble2: argumentos inválidos"


# MARK: Class Position
class Position:
    @staticmethod
    def is_valid_position(arg1: any, arg2: any) -> bool:
        return type(arg1) is int and type(arg2) is int and 1 <= arg1 <= Board.SIZE and 1 <= arg2 <= Board.SIZE

    def __init__(self, line: int, column: int):
        if not Position.is_valid_position(line, column):
            raise ValueError(ErrorMessages.INVALID_POSITION)

        self.__line = line
        self.__column = column

    def __eq__(self, other) -> bool:
        return type(other) is type(self) and self.__line == other.__line and self.__column == other.__column

    def __str__(self) -> str:
        return f"({self.__line},{self.__column})"

    @property
    def line(self) -> int:
        return self.__line

    @property
    def column(self) -> int:
        return self.__column

    def offset(self, direction: str, offset: int) -> "Position":
        new_line, new_column = self.line, self.column
        match direction:
            case Direction.HORIZONTAL:
                new_column += offset
            case Direction.VERTICAL:
                new_line += offset

        if not Position.is_valid_position(new_line, new_column):
            return self

        return Position(new_line, new_column)

    def offset_includes_center(self, direction: str, offset: int) -> bool:
        match direction:
            case Direction.HORIZONTAL:
                return self.line == Board.CENTER and Board.CENTER in range(self.column, self.column + offset + 1)
            case Direction.VERTICAL:
                return self.column == Board.CENTER and Board.CENTER in range(self.line, self.line + offset + 1)

        return False


# MARK: Class Player
class Player:
    MAX_LETTERS = 7

    def __init__(self, id: str) -> None:
        self.__id = id
        self.__letters = {}
        self.__score = 0

    def __eq__(self, other) -> bool:
        return (
            type(other) is type(self)
            and self.__id == other.__id
            and self.__letters == other.__letters
            and self.__score == other.__score
        )

    @property
    def id(self) -> str:
        return self.__id

    @property
    def letters(self) -> dict:
        return self.__letters

    @letters.setter
    def letters(self, value: dict) -> None:
        self.__letters = value

    @property
    def score(self) -> int:
        return self.__score

    @score.setter
    def score(self, value: int) -> None:
        self.__score = value

    def letters_str(self) -> str:
        return "".join([letter * count for letter, count in self.letters.items()])

    def add_letter(self, letter: str) -> "Player":
        self.__letters[letter] = self.__letters.get(letter, 0) + 1
        return self

    def use_letter(self, letter: str) -> "Player":
        self.__letters[letter] = self.__letters.get(letter, 0) - 1
        return self

    def add_score(self, points: int) -> "Player":
        self.__score += points
        return self

    def score_letters_str(self) -> str:
        return f" ({self.__score:3d}):" + "".join(
            (" " + " ".join(letter * self.__letters[letter]))
            for letter in filter(
                lambda letter: self.__letters[letter] > 0, sorted(self.__letters.keys(), key=Vocabulary.ALPHABET.index)
            )
        )

    def deal_letters(self, letters: list[str], max_dealt: int) -> "Player":
        for _ in range(min(max_dealt, len(letters))):
            self.add_letter(letters.pop())
        return self


# MARK: Class Human
class Human(Player):
    """Represents a human player in the Scrabble game."""

    @staticmethod
    def is_valid_human(arg: any) -> bool:
        return type(arg) is str and len(arg) > 0

    def __init__(self, name: str) -> None:
        if not self.is_valid_human(name):
            raise ValueError(ErrorMessages.INVALID_HUMAN)

        super().__init__(name)

    def __str__(self) -> str:
        return self.id + self.score_letters_str()


# MARK: Class Bot
class Bot(Player):
    DIFFICULTY = {
        "FACIL": 100,
        "MEDIO": 50,
        "DIFICIL": 10,
    }

    @staticmethod
    def is_valid_bot(arg: any) -> bool:
        return type(arg) is str and arg in Bot.DIFFICULTY.keys()

    def __init__(self, difficulty: str) -> None:
        if not self.is_valid_bot(difficulty):
            raise ValueError(ErrorMessages.INVALID_BOT)

        super().__init__(difficulty)
        self.__dumbness = self.DIFFICULTY[difficulty]

    @property
    def dumbness(self) -> int:
        return self.__dumbness

    def __str__(self) -> str:
        return f"BOT({self.id})" + self.score_letters_str()


# MARK: Class Vocabulary


class Vocabulary:
    ALPHABET = "ABCÇDEFGHIJLMNOPQRSTUVXZ"
    PLACEHOLDER = "."
    NO_WORD_FOUND = ("", 0)
    SCORE_REFERENCE = {
        "A": 1,
        "B": 3,
        "C": 2,
        "Ç": 3,
        "D": 2,
        "E": 1,
        "F": 4,
        "G": 4,
        "H": 4,
        "I": 1,
        "J": 5,
        "L": 2,
        "M": 1,
        "N": 3,
        "O": 1,
        "P": 2,
        "Q": 6,
        "R": 1,
        "S": 1,
        "T": 1,
        "U": 1,
        "V": 4,
        "X": 8,
        "Z": 8,
    }

    @staticmethod
    def is_valid_word(word: str) -> bool:
        return (
            type(word) is str and 2 <= len(word) <= Board.SIZE and all(letter in Vocabulary.ALPHABET for letter in word)
        )

    @staticmethod
    def is_valid_words(words: tuple[str]) -> bool:
        return type(words) is tuple and all(Vocabulary.is_valid_word(word) for word in words)

    @staticmethod
    def is_uppercase_letter(arg: any) -> bool:
        return type(arg) is str and len(arg) == 1 and arg in Vocabulary.ALPHABET

    def __init__(self, words: tuple[str]) -> None:
        if not self.is_valid_words(words):
            raise ValueError(ErrorMessages.INVALID_VOCABULARY)

        self.words = {}
        for word in set(words):
            self.words.setdefault(len(word), {}).setdefault(word[0], set()).add(word)

    def __str__(self) -> str:
        return "\n".join(
            sorted(
                (
                    word
                    for length in self.words
                    for first_letter in self.words[length]
                    for word in self.words[length][first_letter]
                ),
                key=lambda word: (
                    len(word),
                    Vocabulary.ALPHABET.index(word[0]),
                    -self.get_word_score(word),
                    tuple(Vocabulary.ALPHABET.index(letter) for letter in word),
                ),
            )
        )

    @staticmethod
    def parse_file(filename: str) -> "Vocabulary":
        vocabulary = Vocabulary(())
        with open(filename, "r", encoding="utf-8") as file:
            text = file.read()
        for line in text.splitlines():
            if Vocabulary.is_valid_word(word := line.strip().upper()):
                vocabulary.words.setdefault(len(word), {}).setdefault(word[0], set()).add(word)
        return vocabulary

    def in_vocabulary(self, word: str) -> bool:
        return (len(word) in self.words
                and word[0] in self.words[len(word)]
                and word in self.words[len(word)][word[0]])

    def get_word_score(self, word: str) -> int:
        return sum(Vocabulary.SCORE_REFERENCE[letter] for letter in word) if self.in_vocabulary(word) else 0

    def get_words(self, length: int, first_letter: str) -> tuple[tuple[str, int]]:
        return tuple(
            sorted(
                ((word, self.get_word_score(word)) for word in self.words.get(length, {}).get(first_letter, set())),
                key=lambda word_score: (
                    -word_score[1],
                    tuple(Vocabulary.ALPHABET.index(letter) for letter in word_score[0]),
                ),
            )
        )

    def test_word_pattern(self, word: str, pattern: str, letters: str) -> bool:
        if not (len(word) == len(pattern) and self.in_vocabulary(word)):
            return False

        letters_left = list(letters)
        for word_character, pattern_character in zip(word, pattern):
            if word_character != pattern_character:
                if pattern_character != Vocabulary.PLACEHOLDER or word_character not in letters_left:
                    return False
                letters_left.remove(word_character)
        return True

    def search_word_pattern(self, pattern: str, letters: str, minimum_points: int) -> tuple[str, int]:
        return next(
            iter(
                sorted(
                    (
                        word_score
                        for first_letter in set(letters)
                        for word_score in self.get_words(len(pattern), first_letter)
                        if word_score[1] >= minimum_points
                        and self.test_word_pattern(
                            word_score[0],
                            f"{first_letter}{pattern[1:]}",
                            (letters[: (index := letters.index(first_letter))] + letters[index + 1 :])
                            if first_letter in letters
                            else letters,
                        )
                    )
                    if pattern[0] == Vocabulary.PLACEHOLDER
                    else (
                        word_score
                        for word_score in self.get_words(len(pattern), pattern[0])
                        if word_score[1] >= minimum_points and self.test_word_pattern(word_score[0], pattern, letters)
                    ),
                    key=lambda word_score: (
                        -word_score[1],
                        tuple(Vocabulary.ALPHABET.index(letter) for letter in word_score[0]),
                    ),
                )
            ),
            self.NO_WORD_FOUND,
        )


# MARK: Class Board


class Board:
    SIZE = 15
    CENTER = 15 // 2 + 1

    def __init__(self) -> None:
        self.grid = [[Vocabulary.PLACEHOLDER for _ in range(self.SIZE)] for _ in range(self.SIZE)]

    def __eq__(self, other) -> bool:
        return type(other) is type(self) and self.grid == other.grid

    def __str__(self) -> str:
        return "\n".join(
            [
                "                       1 1 1 1 1 1",
                "     1 2 3 4 5 6 7 8 9 0 1 2 3 4 5",
                "   +-------------------------------+",
                *(f"{i:2d} | {' '.join(row)} |" for i, row in enumerate(self.grid, start=1)),
                "   +-------------------------------+",
            ]
        )

    def get(self, position: Position) -> str:
        return self.grid[position.line - 1][position.column - 1]

    def set(self, position: Position, letter: str) -> "Board":
        self.grid[position.line - 1][position.column - 1] = letter
        return self

    def is_empty(self) -> bool:
        return all(
            self.get(Position(row + 1, column + 1)) == Vocabulary.PLACEHOLDER
            for row in range(self.SIZE)
            for column in range(self.SIZE)
        )

    def get_pattern(self, start_position: Position, end_position: Position) -> str:
        return "".join(
            (
                self.get(Position(start_position.line, column))
                for column in range(start_position.column, end_position.column + 1)
            )
            if start_position.line == end_position.line
            else (
                self.get(Position(line, start_position.column))
                for line in range(start_position.line, end_position.line + 1)
            )
        )

    def insert_word(self, start_position: Position, direction: str, word: str) -> "Board":
        for offset, letter in enumerate(word):
            self.set(start_position.offset(direction, offset), letter)
        return self

    def get_subpatterns(
        self, start_position: Position, end_position: Position, max_free_slots: int
    ) -> tuple[tuple[str], tuple[Position]]:
        pattern = self.get_pattern(start_position, end_position)
        subpatterns, start_positions = [], []

        for i in range(len(pattern)):
            for j in range(len(pattern), i, -1):
                if (
                    pattern[i:j].count(Vocabulary.PLACEHOLDER) > max_free_slots
                    or all(letter == Vocabulary.PLACEHOLDER for letter in pattern[i:j])
                    or all(letter != Vocabulary.PLACEHOLDER for letter in pattern[i:j])
                    or (i > 0 and pattern[i - 1] != Vocabulary.PLACEHOLDER)
                    or (j < len(pattern) and pattern[j] != Vocabulary.PLACEHOLDER)
                ):
                    continue

                subpatterns.append(pattern[i:j])
                start_positions.append(
                    Position(start_position.line, start_position.column + i)
                    if start_position.line == end_position.line
                    else Position(start_position.line + i, start_position.column)
                )

        return tuple(subpatterns), tuple(start_positions)

    def generate_all_patterns(self, max_free_slots: int) -> tuple[tuple[str], tuple[Position], tuple[Direction]]:
        patterns, start_positions, directions = [], [], []

        for i in range(1, self.SIZE + 1):
            subpatterns = self.get_subpatterns(Position(i, 1), Position(i, self.SIZE), max_free_slots)
            patterns.extend(subpatterns[0])
            start_positions.extend(subpatterns[1])
            directions.extend([Direction.HORIZONTAL] * len(subpatterns[0]))

        for i in range(1, self.SIZE + 1):
            subpatterns = self.get_subpatterns(Position(1, i), Position(self.SIZE, i), max_free_slots)
            patterns.extend(subpatterns[0])
            start_positions.extend(subpatterns[1])
            directions.extend([Direction.VERTICAL] * len(subpatterns[0]))

        return tuple(patterns), tuple(start_positions), tuple(directions)


# MARK: Class Scrabble


class Scrabble:
    BOT_TAG = "@"
    INITIAL_SACK = {
        "A": 14,
        "B": 3,
        "C": 4,
        "Ç": 2,
        "D": 5,
        "E": 11,
        "F": 2,
        "G": 2,
        "H": 2,
        "I": 10,
        "J": 2,
        "L": 5,
        "M": 6,
        "N": 4,
        "O": 10,
        "P": 4,
        "Q": 1,
        "R": 6,
        "S": 8,
        "T": 5,
        "U": 7,
        "V": 2,
        "X": 1,
        "Z": 1,
    }

    def __init__(self):
        self.__board = Board()
        self.__players = []
        self.__vocabulary = Vocabulary(())
        self.__sack = []

    @property
    def board(self) -> Board:
        return self.__board

    @board.setter
    def board(self, value: Board) -> None:
        self.__board = value

    @property
    def players(self) -> list[Player]:
        return self.__players

    @players.setter
    def players(self, value: list[Player]) -> None:
        self.__players = value

    @property
    def vocabulary(self) -> Vocabulary:
        return self.__vocabulary

    @vocabulary.setter
    def vocabulary(self, value: Vocabulary) -> None:
        self.__vocabulary = value

    @property
    def sack(self) -> list[str]:
        return self.__sack

    @sack.setter
    def sack(self, value: dict[str, int]) -> None:
        self.__sack = value

    def shuffle_sack(self, seed: int) -> list[str]:
        sack = []
        for letter, quantity in sorted(self.INITIAL_SACK.items(), key=lambda item: Vocabulary.ALPHABET.index(item[0])):
            sack.extend([letter] * quantity)

        for i in range(len(sack) - 1, 0, -1):
            seed ^= (seed << 13) & 0xFFFFFFFF
            seed ^= (seed >> 17) & 0xFFFFFFFF
            seed ^= (seed << 5) & 0xFFFFFFFF
            j = seed % (i + 1)
            sack[i], sack[j] = sack[j], sack[i]

        self.sack = sack
        return sack

    def human_play(self, human: Human) -> bool:
        while True:
            match input(f"Jogada {human.id}: ").split():
                case ["P"]:
                    return False
                case ["T", *sequence]:
                    if not (
                        len(self.sack) >= human.MAX_LETTERS
                        and all(sequence.count(letter) <= human.letters.get(letter, 0) for letter in self.sack)
                    ):
                        continue
                    for letter in sequence:
                        human.use_letter(letter)
                    human.deal_letters(self.sack, human.MAX_LETTERS - sum(human.letters.values()))
                    return True
                case ["J", row, column, direction, word]:
                    if not (
                        row.isdigit()
                        and column.isdigit()
                        and (1 <= int(row) <= self.board.SIZE)
                        and (1 <= int(column) <= self.board.SIZE)
                        and (direction == Direction.HORIZONTAL or direction == Direction.VERTICAL)
                        and all(Vocabulary.is_uppercase_letter(letter) for letter in word)
                        and (position := Position(int(row), int(column))) != position.offset(direction, len(word) - 1)
                        and self.vocabulary.test_word_pattern(
                            word,
                            pattern := self.board.get_pattern(position, position.offset(direction, len(word) - 1)),
                            human.letters_str(),
                        )
                        and (
                            (self.board.is_empty() and position.offset_includes_center(direction, len(word)))
                            or (not self.board.is_empty() and not all(letter == "." for letter in pattern))
                        )
                    ):
                        continue

                    self.board.insert_word(position, direction, word)
                    placed = list(word)
                    for letter in pattern:
                        if letter in placed:
                            placed.remove(letter)
                    for letter in word:
                        human.add_score(Vocabulary.SCORE_REFERENCE[letter])
                    for letter in placed:
                        human.use_letter(letter)
                    human.deal_letters(self.sack, human.MAX_LETTERS - sum(human.letters.values()))
                    return True
                case _:
                    continue

    def bot_play(self, bot: Bot) -> bool:
        output = f"Jogada {bot.id}: "
        if self.board.is_empty():
            print(f"{output}P")
            return False

        patterns = self.board.generate_all_patterns(sum(bot.letters.values()))

        best_word_score = 0
        for pattern, start_position, direction in tuple(zip(*patterns))[:: bot.dumbness]:
            new_word, new_word_score = self.vocabulary.search_word_pattern(
                pattern, bot.letters_str(), best_word_score + 1
            )
            if (new_word, new_word_score) == self.vocabulary.NO_WORD_FOUND:
                continue

            best_word = new_word
            best_word_score = new_word_score
            best_word_pattern = pattern
            best_word_start_position = start_position
            best_word_direction = direction

        if best_word_score == 0:
            if len(self.sack) < bot.MAX_LETTERS:
                print(f"{output}P")
                return False
            output += "T"
            for letter, count in sorted(bot.letters.items(), key=lambda item: self.vocabulary.ALPHABET.index(item[0])):
                for _ in range(count):
                    output += f" {letter}"
                    bot.use_letter(letter)
            bot.deal_letters(self.sack, bot.MAX_LETTERS)
            print(output)
            return True

        self.board.insert_word(best_word_start_position, best_word_direction, best_word)
        placed = list(best_word)
        for letter in best_word_pattern:
            if letter in placed:
                placed.remove(letter)
        for letter in best_word:
            bot.add_score(Vocabulary.SCORE_REFERENCE[letter])
        for letter in placed:
            bot.use_letter(letter)
        bot.deal_letters(self.sack, bot.MAX_LETTERS - sum(bot.letters.values()))
        print(
            f"{output}J"
            + f" {best_word_start_position.line}"
            + f" {best_word_start_position.column}"
            + f" {best_word_direction}"
            + f" {best_word}"
        )
        return True

    def play(self, players: tuple[str], filename: str, seed: int) -> tuple[int]:
        if not (
            type(players) is tuple
            and 2 <= len(players) <= 4
            and all(
                Human.is_valid_human(player)
                or (type(player) is str and len(player) > 1 and Bot.is_valid_bot(player[1:]))
                for player in players
            )
            and type(filename) is str
            and len(filename) > 0
            and type(seed) is int
        ):
            raise ValueError(ErrorMessages.INVALID_SCRABBLE)

        print("Bem-vindo ao SCRABBLE2.")

        game = Scrabble()
        game.vocabulary = Vocabulary.parse_file(filename)
        game.shuffle_sack(seed)

        for player in players:
            if player[0] == game.BOT_TAG:
                new_player = Bot(player[1:])
            else:
                new_player = Human(player)
            new_player.deal_letters(game.sack, Player.MAX_LETTERS)
            game.players.append(new_player)

        current_player_index = 0
        consecutive_passes = 0
        while not (
            consecutive_passes >= len(game.players)
            or (len(game.sack) == 0 and any(len(player.letters) == 0 for player in game.players))
        ):
            print(str(game.board))
            print("\n".join(str(player) for player in game.players))
            current_player = game.players[current_player_index]
            if isinstance(current_player, Human):
                played = game.human_play(current_player)
            else:
                played = game.bot_play(current_player)

            if played:
                consecutive_passes = 0
            else:
                consecutive_passes += 1

            current_player_index = (current_player_index + 1) % len(game.players)

        return tuple(player.score for player in game.players)


# MARK: ADT Position


def cria_casa(line: int, column: int) -> Position:
    return Position(line, column)


def obtem_col(position: Position) -> int:
    return position.column


def obtem_lin(position: Position) -> int:
    return position.line


def eh_casa(arg: any) -> bool:
    return isinstance(arg, Position)


def casas_iguais(arg1: any, arg2: any) -> bool:
    return eh_casa(arg1) and arg1 == arg2


def casa_para_str(position: Position) -> str:
    return str(position)


def str_para_casa(string: str) -> Position:
    return Position(*[int(x) for x in string.strip("()").split(",")])


def incrementa_casa(position: Position, direction: str, offset: int) -> Position:
    return position.offset(direction, offset)


# MARK: ADT Player


def cria_humano(name: str) -> Human:
    return Human(name)


def cria_agente(difficulty: str) -> Bot:
    return Bot(difficulty)


def jogador_identidade(player: Player) -> str:
    return player.id


def jogador_pontos(player: Player) -> int:
    return player.score


def jogador_letras(player: Player) -> dict:
    return player.letters


def recebe_letra(player: Player, letter: str) -> Player:
    return player.add_letter(letter)


def usa_letra(player: Player, letter: str) -> Player:
    return player.use_letter(letter)


def soma_pontos(player: Player, points: int) -> Player:
    return player.add_score(points)


def eh_jogador(arg: any) -> bool:
    return isinstance(arg, Player)


def eh_humano(arg: any) -> bool:
    return isinstance(arg, Human)


def eh_agente(arg: any) -> bool:
    return isinstance(arg, Bot)


def jogadores_iguais(arg1: any, arg2: any) -> bool:
    return eh_jogador(arg1) and arg1 == arg2


def jogador_para_str(player: Player) -> str:
    return str(player)


def distribui_letras(player: Player, letters: list[str], max_dealt: int) -> Player:
    return player.deal_letters(letters, max_dealt)


# MARK: ADT Vocabulary


def cria_vocabulario(words: tuple[str]) -> Vocabulary:
    return Vocabulary(words)


def obtem_pontos(vocabulary: Vocabulary, word: str) -> int:
    return vocabulary.get_word_score(word)


def obtem_palavras(vocabulary: Vocabulary, length: int, first_letter: str) -> tuple[tuple[str, int]]:
    return vocabulary.get_words(length, first_letter)


def testa_palavra_padrao(vocabulary: Vocabulary, word: str, pattern: str, letters: str) -> bool:
    return vocabulary.test_word_pattern(word, pattern, letters)


def ficheiro_para_vocabulario(filename: str) -> Vocabulary:
    return Vocabulary.parse_file(filename)


def vocabulario_para_str(vocabulary: Vocabulary) -> str:
    return str(vocabulary)


def procura_palavra_padrao(vocabulary: Vocabulary, pattern: str, letters: str, minimum_points: int) -> tuple[str, int]:
    return vocabulary.search_word_pattern(pattern, letters, minimum_points)


# MARK: ADT Board


def cria_tabuleiro() -> Board:
    return Board()


def obtem_letra(board: Board, position: Position) -> str:
    return board.get(position)


def insere_letra(board: Board, position: Position, letter: str) -> None:
    return board.set(position, letter)


def eh_tabuleiro(arg: any) -> bool:
    return isinstance(arg, Board)


def eh_tabuleiro_vazio(board: Board) -> bool:
    return board.is_empty()


def tabuleiros_iguais(arg1: any, arg2: any) -> bool:
    return eh_tabuleiro(arg1) and arg1 == arg2


def tabuleiro_para_str(board: Board) -> str:
    return str(board)


def obtem_padrao(board: Board, start_position: Position, end_position: Position) -> str:
    return board.get_pattern(start_position, end_position)


def insere_palavra(board: Board, start_position: Position, direction: str, word: str) -> Board:
    return board.insert_word(start_position, direction, word)


def obtem_subpadroes(
    board: Board, start_position: Position, end_position: Position, max_free_slots: int
) -> tuple[tuple[str], tuple[Position]]:
    return board.get_subpatterns(start_position, end_position, max_free_slots)


def gera_todos_padroes(board: Board, max_free_slots: int) -> tuple[tuple[str], tuple[Position], tuple[Direction]]:
    return board.generate_all_patterns(max_free_slots)


# MARK: Additional Functions


def baralha_saco(seed: int) -> list[str]:
    return Scrabble().shuffle_sack(seed)


def jogada_humano(board: Board, player: Player, vocabulary: Vocabulary, sack: list[str]) -> bool:
    game = Scrabble()
    game.board = board
    game.players.append(player)
    game.vocabulary = vocabulary
    game.sack = sack
    return game.human_play(player)


def jogada_agente(board: Board, player: Player, vocabulary: Vocabulary, sack: list[str]) -> bool:
    game = Scrabble()
    game.board = board
    game.players.append(player)
    game.vocabulary = vocabulary
    game.sack = sack
    return game.bot_play(player)


def scrabble2(players: tuple[str], filename: str, seed: int) -> tuple[int]:
    return Scrabble().play(players, filename, seed)