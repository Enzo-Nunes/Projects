#include "L1Cache.h"

uint8_t DRAM[DRAM_SIZE];
uint32_t time;
Cache L1Cache;

/**************** Time Manipulation ***************/
void resetTime() { time = 0; }

uint32_t getTime() { return time; }

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {

	if (address >= DRAM_SIZE - WORD_SIZE + 1) exit(-1);

	if (mode == MODE_READ) {
		memcpy(data, &(DRAM[address]), BLOCK_SIZE);
		time += DRAM_READ_TIME;
	}

	if (mode == MODE_WRITE) {
		memcpy(&(DRAM[address]), data, BLOCK_SIZE);
		time += DRAM_WRITE_TIME;
	}
}

/*********************** L1 cache *************************/

void initCache() {
	for (int i = 0; i < L1_LINES; i++) {
		CacheLine *Line = &L1Cache.lines[i];
		Line->Valid = 0;
		Line->Dirty = 0;
	}
}

void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {
	uint32_t tag, index, offset, MemAddress;

	index = (address / BLOCK_SIZE) % L1_LINES;		  // Calculate index
	tag = address / (L1_SIZE);						  // Calculate tag
	offset = address % BLOCK_SIZE;					  // Calculate offset
	MemAddress = (address / BLOCK_SIZE) * BLOCK_SIZE; // Address of the block in memory

	uint8_t TempBlock[BLOCK_SIZE];
	CacheLine *Line = &L1Cache.lines[index];

	if (!Line->Valid || Line->Tag != tag) {			  // if block not present - miss - replace with the correct block
		accessDRAM(MemAddress, TempBlock, MODE_READ); // get new block from DRAM

		if ((Line->Valid) && (Line->Dirty)) { // line has dirty block
			uint32_t oldMemAddress = (Line->Tag * L1_LINES + index) * BLOCK_SIZE;
			accessDRAM(oldMemAddress, (Line->Bytes), MODE_WRITE); // then write back old block
		}

		memcpy(Line->Bytes, TempBlock, BLOCK_SIZE); // copy new block to cache line
		Line->Valid = 1;
		Line->Tag = tag;
		Line->Dirty = 0;
	}

	if (mode == MODE_READ) { // read data from cache line
		memcpy(data, (Line->Bytes) + offset, WORD_SIZE);
		time += L1_READ_TIME;
	}

	if (mode == MODE_WRITE) { // write data from cache line
		memcpy((Line->Bytes) + offset, data, WORD_SIZE);
		time += L1_WRITE_TIME;
		Line->Dirty = 1;
	}
}

void read(uint32_t address, uint8_t *data) { accessL1(address, data, MODE_READ); }

void write(uint32_t address, uint8_t *data) { accessL1(address, data, MODE_WRITE); }
