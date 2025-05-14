/**
 * @file hashing.c
 * @brief Implementation of the hash table data structure for this project.
 * @author ist1106336 (Enzo Nunes)
 */

#include "main.h"

HashTable *createHashTable(int hashTableSize, int type) {
	HashTable *hashTable = malloc(sizeof(HashTable));
	hashTable->entries = malloc(sizeof(HashTableEntry *) * hashTableSize);
	for (int i = 0; i < hashTableSize; i++) { hashTable->entries[i] = NULL; }
	hashTable->size = hashTableSize;
	hashTable->type = type;
	return hashTable;
}

unsigned int hash(char *key, int hashTableSize) {
	unsigned int value = 0;
	for (char *p = key; *p != '\0'; p++) { value = value * 37 + *p; }
	return value % hashTableSize;
}

void insert(HashTable *hashTable, void *value) {
	HashTableEntry *newEntry = malloc(sizeof(HashTableEntry));
	if (hashTable->type == VEHICLES) {
		Vehicle *newVehicle = (Vehicle *)value;
		newEntry->key = newVehicle->plate;
		newEntry->value.vehicle = newVehicle;
		newEntry->next = NULL;
	} else {
		ParkingLot *newParkingLot = (ParkingLot *)value;
		newEntry->key = newParkingLot->name;
		newEntry->value.parkingLot = newParkingLot;
		newEntry->next = NULL;
	}
	unsigned int slot = hash(newEntry->key, hashTable->size);
	HashTableEntry *entry = hashTable->entries[slot];
	if (entry == NULL) {
		hashTable->entries[slot] = newEntry;
	} else {
		while (entry->next != NULL) { entry = entry->next; }
		entry->next = newEntry;
	}
}

HashTableEntry *search(HashTable *hashTable, char *key) {
	unsigned int slot = hash(key, hashTable->size);
	HashTableEntry *entry = hashTable->entries[slot];
	while (entry != NULL && strcmp(entry->key, key) != 0) {
		entry = entry->next;
	}
	return entry;
}

void removeEntry(HashTable *hashTable, char *key) {
	unsigned int slot = hash(key, hashTable->size);
	HashTableEntry *entry = hashTable->entries[slot];
	if (strcmp(entry->key, key) == 0) {
		hashTable->entries[slot] = entry->next;
		free(entry);
	} else {
		while (strcmp(entry->next->key, key) != 0) { entry = entry->next; }
		HashTableEntry *temp = entry->next;
		entry->next = entry->next->next;
		free(temp);
	}
}

void freeHashTable(HashTable *hashTable) {
	for (int i = 0; i < hashTable->size; i++) {
		HashTableEntry *entry = hashTable->entries[i];
		while (entry != NULL) {
			HashTableEntry *temp = entry;
			entry = entry->next;
			free(temp);
		}
	}
	free(hashTable->entries);
	free(hashTable);
}