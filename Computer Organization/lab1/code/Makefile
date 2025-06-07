CC = gcc
CFLAGS=-Wall -Wextra
L1TARGET=L1Cache
L2TARGET=L2Cache
L2SETTARGET=L2SetCache

all:
	$(CC) $(CFLAGS) Task1/L1Cache.c Task1/SimpleProgram.c -o $(L1TARGET)
	$(CC) $(CFLAGS) Task2/L2Cache.c Task2/SimpleProgram.c -o $(L2TARGET)
	$(CC) $(CFLAGS) Task3/L2SetCache.c Task3/SimpleProgram.c -o $(L2SETTARGET)

clean:
	rm $(L1TARGET) $(L2TARGET) $(L2SETTARGET)