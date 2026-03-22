TARGET		= project2.cpp
TARGET_EXEC = project2.out
CFLAGS		= -std=c++11 -O3 -Wall -lm

.PHONY: all build

all: build

build:	$(TARGET_EXEC)

$(TARGET_EXEC): $(TARGET)
	g++ $(CFLAGS) $< -o $@