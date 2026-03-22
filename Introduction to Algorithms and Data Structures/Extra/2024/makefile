CC = gcc
CFLAGS = -O3 -g -Wall -Wextra -Werror -Wno-unused-result
VALFLAGS = --leak-check=full --track-origins=yes --show-leak-kinds=all -s
SRC = src/*.c
EXEC = build/project.out

$(EXEC): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean run

clean:
	rm -f $(EXEC)

run: $(EXEC)
	./$(EXEC)

valgrind: $(EXEC)
	valgrind $(VALFLAGS) ./$<