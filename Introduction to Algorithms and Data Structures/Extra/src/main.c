/**
 * @file main.c
 * @brief Main source file for the IAED-24 project.
 * @author ist1106336 (Enzo Nunes)
 */

#include "main.h"

// Parking Lots held both in a hash table and an array for ease of access. Also,
// the count of parking lots.
HashTable *parkingLots;
ParkingLot *parkingLotsArray[PARKING_LOT_HASHTABLE_SIZE];
int parkingLotCount = 0;

// Vehicles hash table and count.
HashTable *vehicles;
int vehicleCount = 0;

// Current date. Input dates must be after each other.
Date currentDate;

void addParkingLot(char *name, int capacity, double priceInit, double priceMain,
				   double price24) {

	ParkingLot *parkingLot = malloc(sizeof(ParkingLot));
	parkingLot->name = name;
	parkingLot->capacity = capacity;
	parkingLot->remaining = capacity;
	parkingLot->priceInit = priceInit;
	parkingLot->priceMain = priceMain;
	parkingLot->price24 = price24;
	parkingLot->logs = NULL;

	parkingLotsArray[parkingLotCount++] = parkingLot;
	insert(parkingLots, parkingLot);
}

int isValidParkingLot(char *name, int capacity, double priceInit,
					  double priceMain, double price24) {
	if (parkingLotCount == PARKING_LOT_HASHTABLE_SIZE) {
		printf(ERR_TOO_MANY_PARKS);
		return FALSE;
	}

	HashTableEntry *entry = search(parkingLots, name);
	if (entry != NULL) {
		printf(ERR_PARKING_LOT_EXISTS, name);
		return FALSE;
	}

	if (capacity <= 0) {
		printf(ERR_INVALID_CAPACITY, capacity);
		return FALSE;
	}

	if (priceInit <= 0 || priceMain <= 0 || price24 <= 0) {
		printf(ERR_INVALID_COST);
		return FALSE;
	}

	if (priceInit > priceMain || priceMain > price24) {
		printf(ERR_INVALID_COST);
		return FALSE;
	}
	return TRUE;
}

void handleCreateParkingLot(char *parkingLotName) {
	char *capacityString = getNextWord();
	char *priceInitString = getNextWord();
	char *priceMainString = getNextWord();
	char *price24String = getNextWord();

	int capacity = atoi(capacityString);
	double priceInit = atof(priceInitString);
	double priceMain = atof(priceMainString);
	double price24 = atof(price24String);

	free(capacityString);
	free(priceInitString);
	free(priceMainString);
	free(price24String);

	if (!isValidParkingLot(parkingLotName, capacity, priceInit, priceMain,
						   price24)) {
		free(parkingLotName);
		return;
	}

	addParkingLot(parkingLotName, capacity, priceInit, priceMain, price24);
}

void showParkingLots() {
	for (int i = 0; i < parkingLotCount; i++) {
		char *name = parkingLotsArray[i]->name;
		int capacity = parkingLotsArray[i]->capacity;
		int free = parkingLotsArray[i]->remaining;
		printf("%s %d %d\n", name, capacity, free);
	}
}

void ManageParkingLot() {
	char *parkingLotName;
	if ((parkingLotName = getNextWord()) == NULL) {
		showParkingLots();
	} else {
		handleCreateParkingLot(parkingLotName);
	}
}

int isValidLicensePlate(char *plate) {
	if (strlen(plate) != 8) { return FALSE; }

	if (plate[2] != '-' || plate[5] != '-') { return FALSE; }

	int digitPairs = 0, letterPairs = 0;
	for (int i = 0; i < 8; i += 3) {
		if (isdigit(plate[i]) && isdigit(plate[i + 1])) {
			digitPairs++;
		} else if (isupper(plate[i]) && isupper(plate[i + 1])) {
			letterPairs++;
		} else {
			return FALSE;
		}
	}

	if (digitPairs < 1 || letterPairs < 1) { return 0; }

	return TRUE;
}

int isValidEntry(ParkingLot *parkingLot, char *parkingLotName, Vehicle *vehicle,
				 char *plate, Date date) {
	if (parkingLot == NULL) {
		printf(ERR_NO_PARKING_LOT, parkingLotName);
		return FALSE;
	}

	if (parkingLot->remaining <= 0) {
		printf(ERR_PARKING_LOT_FULL, parkingLotName);
		return FALSE;
	}

	if (!isValidLicensePlate(plate)) {
		printf(ERR_INVALID_PLATE, plate);
		return FALSE;
	}

	if (vehicle != NULL && vehicle->parkingLot != NULL) {
		printf(ERR_INVALID_ENTRY, plate);
		return FALSE;
	}

	if (!isValidParsedDate(date) || isBefore(date, currentDate)) {
		printf(ERR_INVALID_DATE);
		return FALSE;
	}
	return TRUE;
}

Vehicle *addNewVehicle(char *plate) {
	Vehicle *vehicle = malloc(sizeof(Vehicle));
	vehicle->plate = plate;
	vehicle->parkingLot = NULL;
	vehicle->viaVerde = NULL;
	insert(vehicles, vehicle);
	return vehicle;
}

void logViaVerdeEntry(Vehicle *vehicle, ParkingLot *parkingLot) {
	vehicle->parkingLot = parkingLot;
	if (vehicle->viaVerde == NULL) {
		vehicle->viaVerde = malloc(sizeof(ViaVerde));
		vehicle->viaVerde->parkingLotName = parkingLot->name;
		vehicle->viaVerde->entryDate = currentDate;
		vehicle->viaVerde->exitDate = initDate();
		vehicle->viaVerde->next = NULL;
		vehicle->currentViaVerde = vehicle->viaVerde;
	} else {
		vehicle->currentViaVerde->next = malloc(sizeof(ViaVerde));
		vehicle->currentViaVerde->next->parkingLotName = parkingLot->name;
		vehicle->currentViaVerde->next->entryDate = currentDate;
		vehicle->currentViaVerde->next->exitDate = initDate();
		vehicle->currentViaVerde->next->next = NULL;
		vehicle->currentViaVerde = vehicle->currentViaVerde->next;
	}
}

void logParkingLotEntry(Vehicle *vehicle, ParkingLot *parkingLot) {
	parkingLot->remaining--;
	if (parkingLot->logs == NULL) {
		parkingLot->logs = malloc(sizeof(Log));
		parkingLot->logs->plate = vehicle->plate;
		parkingLot->logs->entryDate = currentDate;
		parkingLot->logs->exitDate = initDate();
		parkingLot->logs->profit = 0;
		parkingLot->logs->next = NULL;
		parkingLot->currentLog = parkingLot->logs;
	} else {
		parkingLot->currentLog->next = malloc(sizeof(Log));
		parkingLot->currentLog->next->plate = vehicle->plate;
		parkingLot->currentLog->next->entryDate = currentDate;
		parkingLot->currentLog->next->exitDate = initDate();
		parkingLot->currentLog->next->profit = 0;
		parkingLot->currentLog->next->next = NULL;
		parkingLot->currentLog = parkingLot->currentLog->next;
	}
	vehicle->currentLog = parkingLot->currentLog;
}

void handleVehicleEntry(ParkingLot *parkingLot, Vehicle *vehicle, char *plate) {
	if (vehicle == NULL) {
		vehicle = addNewVehicle(plate);
	} else {
		free(plate);
	}
	logViaVerdeEntry(vehicle, parkingLot);
	logParkingLotEntry(vehicle, parkingLot);
	printf("%s %d\n", parkingLot->name, parkingLot->remaining);
}

double calculateProfit(Date entryDate, Date exitDate, double priceInit,
					   double priceMain, double price24) {
	size_t minsInDay = 24 * 60;
	size_t timeSpan = getTimeSpan(entryDate, exitDate);
	size_t days = timeSpan / minsInDay;

	double fullDaysprofit = days * price24;
	size_t remaining15Min = ((timeSpan % minsInDay) + 14) / 15; // Rounds up

	double lastDayProfit = 0;
	if (remaining15Min <= 4) {
		lastDayProfit = remaining15Min * priceInit;
	} else {
		lastDayProfit = 4 * priceInit + (remaining15Min - 4) * priceMain;
	}

	fullDaysprofit += min(lastDayProfit, price24);
	return fullDaysprofit;
}

void handleVehicleExit(ParkingLot *parkingLot, Vehicle *vehicle) {
	vehicle->currentViaVerde->exitDate = currentDate;
	vehicle->parkingLot = NULL;
	parkingLot->remaining++;
	Log *exitLog = vehicle->currentLog;
	exitLog->exitDate = currentDate;
	exitLog->profit =
		calculateProfit(exitLog->entryDate, currentDate, parkingLot->priceInit,
						parkingLot->priceMain, parkingLot->price24);
	Date entryDate = vehicle->currentViaVerde->entryDate;

	char *entryDateStr = getDate(entryDate);
	char *entryTimeStr = getTime(entryDate);
	char *currentDateStr = getDate(currentDate);
	char *currentTimeStr = getTime(currentDate);

	printf("%s %s %s %s %s %.2f\n", vehicle->plate, entryDateStr, entryTimeStr,
		   currentDateStr, currentTimeStr, exitLog->profit);

	free(entryDateStr);
	free(entryTimeStr);
	free(currentDateStr);
	free(currentTimeStr);
}

int isValidExit(ParkingLot *parkingLot, char *parkingLotName, Vehicle *vehicle,
				char *plate, Date date) {
	if (parkingLot == NULL) {
		printf(ERR_NO_PARKING_LOT, parkingLotName);
		return FALSE;
	}

	if (!isValidLicensePlate(plate)) {
		printf(ERR_INVALID_PLATE, plate);
		return FALSE;
	}

	if (vehicle == NULL || parkingLot != vehicle->parkingLot) {
		printf(ERR_INVALID_EXIT, plate);
		return FALSE;
	}

	if (!isValidParsedDate(date) || isBefore(date, currentDate)) {
		printf(ERR_INVALID_DATE);
		return FALSE;
	}
	return TRUE;
}

void executeVehicleMovements(char command, ParkingLot *parkingLot,
							 char *parkingLotName, Vehicle *vehicle,
							 char *plate, Date parsedDate) {
	switch (command) {
	case VEHICLE_ENTRY:
		if (!isValidEntry(parkingLot, parkingLotName, vehicle, plate,
						  parsedDate)) {
			free(plate);
			break;
		}
		currentDate = parsedDate;
		handleVehicleEntry(parkingLot, vehicle, plate);
		break;
	case VEHICLE_EXIT:
		if (!isValidExit(parkingLot, parkingLotName, vehicle, plate,
						 parsedDate)) {
			free(plate);
			break;
		}
		currentDate = parsedDate;
		free(plate);
		handleVehicleExit(parkingLot, vehicle);
		break;
	}
}

void handleVehicleMovements(char command) {
	char *parkingLotName = getNextWord();
	char *plate = getNextWord();
	char *date = getNextWord();
	char *time = getNextWord();

	HashTableEntry *entry = search(parkingLots, parkingLotName);
	ParkingLot *parkingLot = (entry == NULL) ? NULL : entry->value.parkingLot;
	entry = search(vehicles, plate);
	Vehicle *vehicle = (entry == NULL) ? NULL : entry->value.vehicle;
	Date parsedDate = parseDate(date, time);

	executeVehicleMovements(command, parkingLot, parkingLotName, vehicle, plate,
							parsedDate);

	free(parkingLotName);
	free(date);
	free(time);
}

ViaVerde *getMiddleViaVerde(ViaVerde *head) {
	if (head == NULL) { return head; }
	ViaVerde *slow = head;
	ViaVerde *fast = head->next;
	while (fast != NULL) {
		fast = fast->next;
		if (fast != NULL) {
			slow = slow->next;
			fast = fast->next;
		}
	}
	return slow;
}

ViaVerde *mergeSortedViaVerde(ViaVerde *a, ViaVerde *b) {
	ViaVerde *result = NULL;
	if (a == NULL) {
		return b;
	} else if (b == NULL) {
		return a;
	}
	if (strcmp(a->parkingLotName, b->parkingLotName) <= 0) {
		result = a;
		result->next = mergeSortedViaVerde(a->next, b);
	} else {
		result = b;
		result->next = mergeSortedViaVerde(a, b->next);
	}
	return result;
}

ViaVerde *sortViaVerde(ViaVerde *head) {
	if (head == NULL || head->next == NULL) { return head; }
	ViaVerde *middle = getMiddleViaVerde(head);
	ViaVerde *nextOfMiddle = middle->next;
	middle->next = NULL;
	ViaVerde *left = sortViaVerde(head);
	ViaVerde *right = sortViaVerde(nextOfMiddle);
	return mergeSortedViaVerde(left, right);
}

void sortVehicleViaVerde(Vehicle *vehicle) {
	vehicle->viaVerde = sortViaVerde(vehicle->viaVerde);
	ViaVerde *currentViaVerde = vehicle->viaVerde;
	while (currentViaVerde->next != NULL) {
		currentViaVerde = currentViaVerde->next;
	}
	vehicle->currentViaVerde = currentViaVerde;
}

int isValidShowVehicleMovements(Vehicle *vehicle, char *plate) {
	if (!isValidLicensePlate(plate)) {
		printf(ERR_INVALID_PLATE, plate);
		return FALSE;
	}

	if (vehicle == NULL || vehicle->viaVerde == NULL) {
		printf(ERR_NO_VIA_VERDE, plate);
		return FALSE;
	}
	return TRUE;
}

void showVehicleMovements(Vehicle *vehicle) {
	for (ViaVerde *viaVerde = vehicle->viaVerde; viaVerde != NULL;
		 viaVerde = viaVerde->next) {
		Date entryDate = viaVerde->entryDate;
		char *entryDateStr = getDate(entryDate);
		char *entryTimeStr = getTime(entryDate);
		printf("%s %s %s", viaVerde->parkingLotName, entryDateStr,
			   entryTimeStr);
		free(entryDateStr);
		free(entryTimeStr);
		if (isValidParsedDate(viaVerde->exitDate)) {
			char *exitDateStr = getDate(viaVerde->exitDate);
			char *exitTimeStr = getTime(viaVerde->exitDate);
			printf(" %s %s", exitDateStr, exitTimeStr);
			free(exitDateStr);
			free(exitTimeStr);
		}
		printf("\n");
	}
}

void handleShowVehicleMovements() {
	char *plate = getNextWord();
	HashTableEntry *entry = search(vehicles, plate);
	Vehicle *vehicle = (entry == NULL) ? NULL : entry->value.vehicle;

	if (!isValidShowVehicleMovements(vehicle, plate)) {
		free(plate);
		return;
	}

	free(plate);

	sortVehicleViaVerde(vehicle);
	showVehicleMovements(vehicle);
}

int isValidShowParkingLotProfit(ParkingLot *parkingLot, char *parkingLotName,
								Date date, char *dateStr) {
	if (parkingLot == NULL) {
		printf(ERR_NO_PARKING_LOT, parkingLotName);
		return FALSE;
	}

	if (dateStr != NULL && !isValidParsedDate(date)) {
		printf(ERR_INVALID_DATE);
		return FALSE;
	}
	return TRUE;
}

Log *getMiddleLog(Log *head) {
	if (head == NULL) { return head; }
	Log *slow = head;
	Log *fast = head->next;
	while (fast != NULL) {
		fast = fast->next;
		if (fast != NULL) {
			slow = slow->next;
			fast = fast->next;
		}
	}
	return slow;
}

Log *mergeSortedLogs(Log *a, Log *b) {
	Log *result = NULL;
	if (a == NULL) {
		return b;
	} else if (b == NULL) {
		return a;
	}
	if (isBefore(a->exitDate, b->exitDate)) {
		result = a;
		result->next = mergeSortedLogs(a->next, b);
	} else {
		result = b;
		result->next = mergeSortedLogs(a, b->next);
	}
	return result;
}

Log *sortLogs(Log *head) {
	if (head == NULL || head->next == NULL) { return head; }
	Log *middle = getMiddleLog(head);
	Log *nextOfMiddle = middle->next;
	middle->next = NULL;
	Log *left = sortLogs(head);
	Log *right = sortLogs(nextOfMiddle);
	return mergeSortedLogs(left, right);
}

void sortParkingLotLogs(ParkingLot *parkingLot) {
	parkingLot->logs = sortLogs(parkingLot->logs);
	Log *currentLog = parkingLot->logs;
	while (currentLog->next != NULL) { currentLog = currentLog->next; }
	parkingLot->currentLog = currentLog;
}

void showAllParkingLotProfit(ParkingLot *parkingLot) {
	double dateProfit = 0;
	sortParkingLotLogs(parkingLot);
	for (Log *log = parkingLot->logs; log != NULL; log = log->next) {
		if (!isValidParsedDate(log->exitDate)) { continue; }
		dateProfit += log->profit;
		while (log->next != NULL &&
			   dateEqualsDate(log->exitDate, log->next->exitDate)) {
			log = log->next;
			dateProfit += log->profit;
		}
		char *exitDateStr = getDate(log->exitDate);
		printf("%s %.2f\n", exitDateStr, dateProfit);
		free(exitDateStr);
		dateProfit = 0;
	}
}

void showParkingLotProfit(ParkingLot *parkingLot, Date date) {
	sortParkingLotLogs(parkingLot);
	for (Log *log = parkingLot->logs; log != NULL; log = log->next) {
		if (!dateEqualsDate(date, log->exitDate)) { continue; }
		char *exitTimeStr = getTime(log->exitDate);
		printf("%s %s %.2f\n", log->plate, exitTimeStr, log->profit);
		free(exitTimeStr);
	}
}

void handleShowParkingLotProfit() {
	char *parkingLotName = getNextWord();
	char *dateStr = getNextWord();
	ParkingLot *parkingLot =
		search(parkingLots, parkingLotName)->value.parkingLot;
	Date date = parseDate(dateStr, MIDNIGHT);

	if (!isValidShowParkingLotProfit(parkingLot, parkingLotName, date,
									 dateStr)) {
		free(parkingLotName);
		free(dateStr);
		return;
	}

	if ((dateStr) == NULL) {
		showAllParkingLotProfit(parkingLot);
	} else {
		showParkingLotProfit(parkingLot, date);
	}

	free(parkingLotName);
	free(dateStr);
}

void removeParkingLotFromVehicle(Vehicle *vehicle, char *parkingLotName) {
	ViaVerde *current = vehicle->viaVerde;
	ViaVerde *previous = NULL;

	while (current != NULL) {
		if (strcmp(current->parkingLotName, parkingLotName) == 0) {
			if (previous == NULL) {
				vehicle->viaVerde = current->next;
			} else {
				previous->next = current->next;
			}
			ViaVerde *toBeFreed = current;
			current = current->next;
			free(toBeFreed);
		} else {
			previous = current;
			current = current->next;
		}
	}

	if (vehicle->parkingLot != NULL &&
		strcmp(vehicle->parkingLot->name, parkingLotName) == 0) {
		vehicle->parkingLot = NULL;
	}
}

void freeParkingLotLogs(ParkingLot *parkingLot) {
	Log *nextLog;
	for (Log *log = parkingLot->logs; log != NULL; log = nextLog) {
		removeParkingLotFromVehicle(search(vehicles, log->plate)->value.vehicle,
									parkingLot->name);
		nextLog = log->next;
		free(log);
	}
}

void removeFromParkingLotsArray(ParkingLot *parkingLot) {
	for (int i = 0; i < parkingLotCount; i++) {
		if (parkingLotsArray[i] == parkingLot) {
			for (int j = i; j < parkingLotCount - 1; j++) {
				parkingLotsArray[j] = parkingLotsArray[j + 1];
			}
			parkingLotCount--;
			break;
		}
	}
}

void printRemainingParkingLots() {
	ParkingLot **sortedParkingLotsArray =
		malloc(parkingLotCount * sizeof(ParkingLot *));
	memcpy(sortedParkingLotsArray, parkingLotsArray,
		   parkingLotCount * sizeof(ParkingLot *));

	// Sort the copied array using Insertion Sort
	for (int i = 1; i < parkingLotCount; i++) {
		ParkingLot *key = sortedParkingLotsArray[i];
		int j = i - 1;

		while (j >= 0 &&
			   strcmp(sortedParkingLotsArray[j]->name, key->name) > 0) {
			sortedParkingLotsArray[j + 1] = sortedParkingLotsArray[j];
			j = j - 1;
		}
		sortedParkingLotsArray[j + 1] = key;
	}

	for (int i = 0; i < parkingLotCount; i++) {
		ParkingLot *parkingLot = sortedParkingLotsArray[i];
		printf("%s\n", parkingLot->name);
	}

	free(sortedParkingLotsArray);
}

void handleRemoveParkingLot() {
	char *parkingLotName = getNextWord();
	HashTableEntry *entry = search(parkingLots, parkingLotName);
	if (entry == NULL) {
		printf(ERR_NO_PARKING_LOT, parkingLotName);
		free(parkingLotName);
		return;
	}

	ParkingLot *parkingLot = entry->value.parkingLot;
	freeParkingLotLogs(parkingLot);
	removeEntry(parkingLots, parkingLotName);
	removeFromParkingLotsArray(parkingLot);

	free(parkingLotName);
	free(parkingLot->name);
	free(parkingLot);

	printRemainingParkingLots();
}

int handleCommand(char command) {

	switch (command) {
	case EXIT:
		return 1;
	case MANAGE_PARKING_LOT:
		ManageParkingLot();
		return 0;
	case VEHICLE_ENTRY:
		handleVehicleMovements(command);
		return 0;
	case VEHICLE_EXIT:
		handleVehicleMovements(command);
		return 0;
	case SHOW_VEHICLE:
		handleShowVehicleMovements();
		return 0;
	case SHOW_PARKING_LOT:
		handleShowParkingLotProfit();
		return 0;
	case REMOVE_PARKING_LOT:
		handleRemoveParkingLot();
		return 0;
	default:
		return 0;
	}
}

void freeParkingLots() {
	for (int i = 0; i < parkingLotCount; i++) {
		ParkingLot *parkingLot = parkingLotsArray[i];
		freeParkingLotLogs(parkingLot);
		free(parkingLot->name);
		free(parkingLot);
	}
}

void freeVehicles() {
	for (int i = 0; i < vehicles->size; i++) {
		for (HashTableEntry *entry = vehicles->entries[i]; entry != NULL;
			 entry = entry->next) {
			Vehicle *vehicle = entry->value.vehicle;
			free(vehicle->plate);
			free(vehicle);
		}
	}
}

void freeSystem() {
	freeParkingLots();
	freeVehicles();
	freeHashTable(parkingLots);
	freeHashTable(vehicles);
}

int main() {
	parkingLots = createHashTable(PARKING_LOT_HASHTABLE_SIZE, PARKING_LOTS);
	vehicles = createHashTable(VEHICLE_HASHTABLE_SIZE, VEHICLES);
	currentDate = initDate();

	int returnValue = 0;
	while (returnValue != 1) {
		char command = readInput();
		returnValue = handleCommand(command);
		clearBuffer();
	}
	freeSystem();
}