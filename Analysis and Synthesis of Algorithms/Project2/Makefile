TARGET		= project2.cpp
TARGET_EXEC = project2.out
OUTBIN		= ./$(TARGET_EXEC)
CFLAGS		= -std=c++11 -O3 -Wall -lm
TEST=tests

TESTS=$(wildcard $(TEST)/*.in)
RESULTS=$(patsubst %.in,%.out,$(TESTS))

.PHONY: all run build test

all:	run

run:	$(TARGET_EXEC)
	./$(TARGET_EXEC) < input2.txt

build:	$(TARGET_EXEC)

test:
	@mkdir -p test_res
	echo "" > test_res/test_effective_out
	for testfile in $(TESTS) ; do \
		$(OUTBIN) < $$testfile >> test_res/test_effective_out; \
	done
	echo "" > test_res/test_expected_out
	for resultfile in $(RESULTS) ; do \
		cat $$resultfile >> test_res/test_expected_out; \
	done
	echo "" > test_res/test_order
	for testfile in $(TESTS) ; do \
		echo $$testfile >> test_res/test_order; \
	done
	diff test_res/test_effective_out test_res/test_expected_out > test_res/00_diff

$(TARGET_EXEC): $(TARGET)
	g++ $(CFLAGS) $< -o $@