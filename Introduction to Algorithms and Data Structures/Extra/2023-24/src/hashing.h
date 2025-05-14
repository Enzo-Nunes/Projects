/**
 * @file hashing.h
 * @brief Implementation of the hash table data structure for this project.
 * @author ist1106336 (Enzo Nunes)
 */

// Hash table types
#define VEHICLES	 1
#define PARKING_LOTS 0

#define PARKING_LOT_HASHTABLE_SIZE \
	20 // Stated in the project guide. Global variable will be used to keep
	   // track of the number of parking lots.
#define VEHICLE_HASHTABLE_SIZE \
	1000 // Note that this doesn't mean that the hash table will have a fixed
		 // size of 1000, as the length of the linked lists used to handle
		 // collisions can increase indefinitely. It's an arbitrary number that
		 // I determined was optimal for this specific project. Memory is still
		 // allocated dynamically.

/**
 * @struct HashTableEntry
 * @brief Represents an entry in a hash table.
 *
 * This struct contains a key, which is a string, and a value, which is a union
 * that can hold either a pointer to a Vehicle struct or a pointer to a
 * ParkingLot struct. It also contains a pointer to the next entry with the same
 * hash in the hash table to handle collisions.
 */
typedef struct HashTableEntry {
	char *key; /**< The key associated with the entry. */
	union {
		struct Vehicle *vehicle;
		struct ParkingLot *parkingLot;
	} value;
	struct HashTableEntry *next;
} HashTableEntry;

/**
 * @struct HashTable
 * @brief Represents a hash table data structure.
 *
 * The `HashTable` struct contains an array of `HashTableEntry` pointers,
 * representing the entries in the hash table. It also includes the type of
 * the hash table, which can be used to store either vehicles or parking lots,
 * and the size of the hash table.
 */
typedef struct HashTable {
	HashTableEntry **entries;
	int type;
	int size;
} HashTable;

/**
 * Creates a new hash table with the specified size and type.
 *
 * @param hashTableSize The size of the hash table.
 * @param type The type of the hash table.
 * @return A pointer to the newly created hash table.
 */
HashTable *createHashTable(int hashTableSize, int type);

/**
 * Hashes the given key respecting the specified hash table size.
 *
 * @param key The key to be hashed.
 * @param hashTableSize The size of the hash table.
 * @return The hash value of the key.
 */
unsigned int hash(char *key, int hashTableSize);

/**
 * Inserts either a vehicle of parking lot into the specified hash table,
 * depending on its type.
 *
 * @param hashTable The hash table to insert the value into.
 * @param value The value to be inserted.
 */
void insert(HashTable *hashTable, void *value);

/**
 * Searches for an entry with the specified key in the hash table.
 *
 * @param hashTable The hash table to search in.
 * @param key The key to search for.
 * @return A pointer to the hash table entry if found, NULL otherwise.
 */
HashTableEntry *search(HashTable *hashTable, char *key);

/**
 * Removes the entry with the specified key from the hash table.
 *
 * @param hashTable The hash table to remove the entry from.
 * @param key The key of the entry to be removed.
 */
void removeEntry(HashTable *hashTable, char *key);

/**
 * Frees the memory allocated for the hash table.
 *
 * @param hashTable The hash table to be freed.
 */
void freeHashTable(HashTable *hashTable);