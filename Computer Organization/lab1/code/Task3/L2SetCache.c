#include "L2SetCache.h"

uint8_t DRAM[DRAM_SIZE];
uint32_t time;
Cache L1Cache;
L2SetCache L2Cache;

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

/*********************** L2 cache *************************/

void accessL2(uint32_t address, uint8_t *data, uint32_t mode) {
	uint32_t tag, index, offset, MemAddress;
	index = (address / BLOCK_SIZE) % L2_SET_COUNT;	  // Calculate index
	tag = address / L2_SIZE;						  // Calculate tag
	offset = address % BLOCK_SIZE;					  // Calculate offset
	MemAddress = (address / BLOCK_SIZE) * BLOCK_SIZE; // Align address to block boundary

	uint8_t TempBlock[BLOCK_SIZE];
	CacheSet *set = &L2Cache.sets[index]; // Access the correct set
	CacheLine *line1 = &set->lines[0];	  // First line in the set
	CacheLine *line2 = &set->lines[1];	  // Second line in the set

	// Increment the LRU counters for both lines (assuming this function is called for every access)
	line1->LRUCounter++;
	line2->LRUCounter++;

	CacheLine *victimLine = NULL;

	// Check both lines in the set for a valid tag match
	if (line1->Valid && line1->Tag == tag) { // Cache hit in the first line
		victimLine = line1;
		line1->LRUCounter = 0;						// Reset LRU counter for the line just accessed
	} else if (line2->Valid && line2->Tag == tag) { // Cache hit in the second line
		victimLine = line2;
		line2->LRUCounter = 0; // Reset LRU counter for the line just accessed
	} else {				   // Cache miss

		// Select victim line using LRU policy: the line with the higher LRU counter is less recently used
		if (line1->LRUCounter > line2->LRUCounter) {
			victimLine = line1;
		} else {
			victimLine = line2;
		}

		// If the victim line is valid and dirty, write it back to DRAM
		if (victimLine->Valid && victimLine->Dirty) {
			// Reconstruct the full memory address from the tag and index
			uint32_t oldMemAddress = (victimLine->Tag * L2_SET_COUNT + index) * BLOCK_SIZE;
			accessDRAM(oldMemAddress, victimLine->Bytes, MODE_WRITE);
		}

		// Fetch the new block from DRAM
		accessDRAM(MemAddress, TempBlock, MODE_READ);
		memcpy(victimLine->Bytes, TempBlock, BLOCK_SIZE); // Copy new block to cache

		// Update the victim line metadata
		victimLine->Valid = 1;
		victimLine->Tag = tag;
		victimLine->Dirty = 0;		// Fresh data from DRAM, so not dirty
		victimLine->LRUCounter = 0; // Reset LRU counter for the line just accessed
	}

	// Access the correct cache line (victimLine is the line we just accessed or updated)
	if (mode == MODE_READ) {
		memcpy(data, (victimLine->Bytes) + offset, WORD_SIZE); // Read the requested data
		time += L2_READ_TIME;
	} else if (mode == MODE_WRITE) {
		memcpy((victimLine->Bytes) + offset, data, WORD_SIZE); // Write the data
		time += L2_WRITE_TIME;
		victimLine->Dirty = 1; // Mark as dirty
	}
}

/*********************** Cache *************************/

void initCache() {
	for (int i = 0; i < L1_LINES; i++) {
		CacheLine *Line = &L1Cache.lines[i];
		Line->Valid = 0;
		Line->Dirty = 0;
	}

	for (int i = 0; i < L2_SET_COUNT; i++) {
		for (int j = 0; j < 2; j++) {
			CacheLine *Line = &L2Cache.sets[i].lines[j];
			Line->Valid = 0;
			Line->Dirty = 0;
			Line->LRUCounter = 0;
		}
	}
}

void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {
	uint32_t tag, index, offset;
	index = (address / BLOCK_SIZE) % L1_LINES; // Calculate index
	tag = address / (L1_SIZE);				   // Calculate tag
	offset = address % BLOCK_SIZE;			   // Calculate offset

	uint8_t TempBlock[BLOCK_SIZE];
	CacheLine *Line = &L1Cache.lines[index];

	if (!Line->Valid || Line->Tag != tag) {		 // if block not present - miss - check L2 to see if it is there
		accessL2(address, TempBlock, MODE_READ); // get new block from L2

		if ((Line->Valid) && (Line->Dirty)) { // line has dirty block
			uint32_t oldMemAddress = (Line->Tag * L1_LINES + index) * BLOCK_SIZE;
			accessL2(oldMemAddress, (Line->Bytes), MODE_WRITE); // then write back old block
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
