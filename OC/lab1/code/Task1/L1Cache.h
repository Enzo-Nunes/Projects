#ifndef SIMPLECACHE_H
#define SIMPLECACHE_H

#include "../Cache.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void resetTime();

uint32_t getTime();

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t, uint8_t *, uint32_t);

/*********************** Cache *************************/

void initCache();
void accessL1(uint32_t, uint8_t *, uint32_t);

typedef struct CacheLine {
	uint8_t Valid;
	uint8_t Dirty;
	uint32_t Tag;
	uint8_t Bytes[BLOCK_SIZE];
} CacheLine;

typedef struct Cache {
	CacheLine lines[L1_LINES]; // Array of cache lines
} Cache;

/*********************** Interfaces *************************/

void read(uint32_t, uint8_t *);

void write(uint32_t, uint8_t *);

#endif
