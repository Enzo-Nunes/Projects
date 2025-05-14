CC = gcc
CFLAGS = -O3 -g -Wall -Wextra -Werror -Wno-unused-result
VALFLAGS = --leak-check=full --track-origins=yes --show-leak-kinds=all -s
SRC = src/*.c
EXEC = build/banana.out

$(EXEC): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean run

clean:
	rm -f $(EXEC)

run: $(EXEC)
	./$(EXEC)

valgrind: $(EXEC)
	valgrind $(VALFLAGS) ./$<

valgrindall: $(EXEC)
	for file in public-tests/*.in; do \
		valgrind $(VALFLAGS) ./$< < $$file > /dev/null; \
	done

gprof:
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS) -pg --no-inline
	cd build && ../$(EXEC) < ../public-tests/test18.in
	gprof $(EXEC) build/gmon.out
	rm -f build/gmon.out