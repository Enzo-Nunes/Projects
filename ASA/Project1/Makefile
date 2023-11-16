TARGET		= project1.cpp
TARGET_EXEC = project1.out
CFLAGS		= -std=c++11 -O3 -Wall  -lm

.PHONY: all run build

all:	run

run:	$(TARGET_EXEC)
	./$(TARGET_EXEC)

build:	$(TARGET_EXEC)

$(TARGET_EXEC): $(TARGET)
	g++ $(CFLAGS) $< -o $@