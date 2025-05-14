/**
 * @file main.h
 * @brief Main header file for the IAED-24 project.
 * @author ist1106336 (Enzo Nunes)
 */

#include "buffer.h"
#include "date.h"
#include "hashing.h"

// Libraries
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macro for minimum value
#define min(a, b) ((a) < (b) ? (a) : (b))

// Boolean values
#define TRUE  1
#define FALSE 0

// Error Strings
#define ERR_PARKING_LOT_EXISTS "%s: parking already exists.\n"
#define ERR_INVALID_CAPACITY   "%d: invalid capacity.\n"
#define ERR_INVALID_COST	   "invalid cost.\n"
#define ERR_TOO_MANY_PARKS	   "too many parks.\n"
#define ERR_NO_PARKING_LOT	   "%s: no such parking.\n"
#define ERR_PARKING_LOT_FULL   "%s: parking is full.\n"
#define ERR_INVALID_PLATE	   "%s: invalid licence plate.\n"
#define ERR_INVALID_ENTRY	   "%s: invalid vehicle entry.\n"
#define ERR_INVALID_EXIT	   "%s: invalid vehicle exit.\n"
#define ERR_INVALID_DATE	   "invalid date.\n"
#define ERR_NO_VIA_VERDE	   "%s: no entries found in any parking.\n"

// Command identifiers
enum Commands {
	MANAGE_PARKING_LOT = 'p',
	VEHICLE_ENTRY = 'e',
	VEHICLE_EXIT = 's',
	SHOW_VEHICLE = 'v',
	SHOW_PARKING_LOT = 'f',
	REMOVE_PARKING_LOT = 'r',
	EXIT = 'q',
};

/**
 * @struct ViaVerde
 * @brief Represents a ViaVerde structure unique to the vehicles that logs their
 * stays in parking lots.
 *
 * The `ViaVerde` struct is a linked list node and contains information about
 * the name of the parking lot, the entry and exit dates, and a pointer to the
 * next ViaVerde structure.
 */
typedef struct ViaVerde {
	char *parkingLotName;
	Date entryDate, exitDate;
	struct ViaVerde *next;
} ViaVerde;

/**
 * @brief Structure representing a vehicle.
 *
 * The `Vehicle` struct contains information about a vehicle, including its
 * license plate, the parking lot where it is parked, and pointers to its
 * ViaVerde. It also includes a pointer to the vehicle's current log in the
 * parking lot.
 */
typedef struct Vehicle {
	char *plate;
	struct ParkingLot *parkingLot;
	struct ViaVerde *viaVerde, *currentViaVerde;
	struct Log *currentLog;
} Vehicle;

/**
 * @struct Log
 * @brief Represents a log structure unique to parking lots that logs the
 * vehicles that stay in the parking lot.
 *
 * The `Log` struct is a linked list node and contains information about a
 * vehicle's entry and exit dates, its license plate, and the profit generated
 * from the vehicle's stay.
 */
typedef struct Log {
	char *plate;
	Date entryDate, exitDate;
	double profit;
	struct Log *next;
} Log;

/**
 * @struct ParkingLot
 * @brief Represents a parking lot.
 *
 * The `ParkingLot` struct contains information about a parking lot, including
 * its name, capacity, remaining spaces, initial price, main price, and 24-hour
 * price. It also includes pointers to the logs of the parking lot.
 */
typedef struct ParkingLot {
	char *name;
	int capacity, remaining;
	double priceInit, priceMain, price24;
	struct Log *logs, *currentLog;
} ParkingLot;

/**
 * @brief Initializes and adds a new parking lot to the system.
 *
 * @param name The name of the parking lot.
 * @param capacity The capacity of the parking lot.
 * @param priceInit The price for the first hour of parking.
 * @param priceMain The main price for parking.
 * @param price24 The maximum daily price for parking.
 */
void addParkingLot(char *name, int capacity, double priceInit, double priceMain,
				   double price24);

/**
 * @brief Checks if the parameters for a parking lot are valid.
 *
 * @param name The name of the parking lot.
 * @param capacity The capacity of the parking lot.
 * @param priceInit The price for the first hour of parking.
 * @param priceMain The main price for parking.
 * @param price24 The maximum daily price for parking.
 * @return 1 if the parameters are valid, 0 otherwise.
 */
int isValidParkingLot(char *name, int capacity, double priceInit,
					  double priceMain, double price24);

/**
 * @brief Handles the creation of a new parking lot.
 *
 * @param parkingLotName The name of the parking lot to be created.
 */
void handleCreateParkingLot(char *parkingLotName);

/**
 * @brief Lists all the parking lots in the system.
 */
void showParkingLots();

/**
 * @brief Manages the operations related to a parking lot. Either creation or
 * listing of parking lots.
 */
void ManageParkingLot();

/**
 * @brief Checks if a license plate is valid.
 *
 * @param plate The license plate to be checked.
 * @return 1 if the license plate is valid, 0 otherwise.
 */
int isValidLicensePlate(char *plate);

/**
 * @brief Checks if the vehicle entry parameters are valid.
 *
 * @param parkingLot The parking lot where the vehicle is trying to enter.
 * @param parkingLotName The name of the parking lot.
 * @param vehicle The vehicle trying to enter.
 * @param plate The license plate of the vehicle.
 * @param date The date of the entry.
 * @return 1 if the entry parameters are valid, 0 otherwise.
 */
int isValidEntry(ParkingLot *parkingLot, char *parkingLotName, Vehicle *vehicle,
				 char *plate, Date date);

/**
 * @brief Initializes and adds a new vehicle to the system.
 *
 * @param plate The license plate of the vehicle.
 * @return A pointer to the newly created vehicle.
 */
Vehicle *addNewVehicle(char *plate);

/**
 * @brief Logs a new vehicle entry in its ViaVerde.
 *
 * @param vehicle The vehicle that entered.
 * @param parkingLot The parking lot where the vehicle entered.
 */
void logViaVerdeEntry(Vehicle *vehicle, ParkingLot *parkingLot);

/**
 * @brief Logs a new vehicle entry in a parking lot.
 *
 * @param vehicle The vehicle that entered.
 * @param parkingLot The parking lot where the vehicle entered.
 */
void logParkingLotEntry(Vehicle *vehicle, ParkingLot *parkingLot);

/**
 * @brief Handles the entry of a vehicle into a parking lot.
 *
 * @param parkingLot The parking lot where the vehicle is trying to enter.
 * @param vehicle The vehicle trying to enter.
 * @param plate The license plate of the vehicle.
 */
void handleVehicleEntry(ParkingLot *parkingLot, Vehicle *vehicle, char *plate);

/**
 * @brief Calculates the profit for a given period of time.
 *
 * @param entryDate The date of entry.
 * @param exitDate The date of exit.
 * @param priceInit The price for the first hour of parking.
 * @param priceMain The main price for parking.
 * @param price24 The maximum daily price for parking.
 * @return The calculated profit.
 */
double calculateProfit(Date entryDate, Date exitDate, double priceInit,
					   double priceMain, double price24);

/**
 * @brief Handles the exit of a vehicle from a parking lot.
 *
 * @param parkingLot The parking lot where the vehicle is exiting from.
 * @param vehicle The vehicle that is exiting.
 */
void handleVehicleExit(ParkingLot *parkingLot, Vehicle *vehicle);

/**
 * @brief Checks if the vehicle exit parameters are valid.
 *
 * @param parkingLot The parking lot where the vehicle is exiting from.
 * @param parkingLotName The name of the parking lot.
 * @param vehicle The vehicle that is exiting.
 * @param plate The license plate of the vehicle.
 * @param date The date of the exit.
 * @return 1 if the exit parameters are valid, 0 otherwise.
 */
int isValidExit(ParkingLot *parkingLot, char *parkingLotName, Vehicle *vehicle,
				char *plate, Date date);

/**
 * @brief Determines if the command is an entry or exit and calls the respective
 * functions.
 *
 * @param command The command to be executed.
 * @param parkingLot The parking lot where the vehicle is located.
 * @param parkingLotName The name of the parking lot.
 * @param vehicle The vehicle to be moved.
 * @param plate The license plate of the vehicle.
 * @param parsedDate The parsed date for the movement.
 */
void executeVehicleMovements(char command, ParkingLot *parkingLot,
							   char *parkingLotName, Vehicle *vehicle,
							   char *plate, Date parsedDate);

/**
 * @brief Handles vehicle movements in the system.
 *
 * @param command The command to be executed. Either entry or exit.
 */
void handleVehicleMovements(char command);

//===================MERGE SORT IMPLEMENTATION FOR VIAVERDE===================//
/**
 * @brief Gets the middle node of a linked list.
 *
 * @param head The head of the linked list.
 * @return The middle node of the linked list.
 */
ViaVerde *getMiddleViaVerde(ViaVerde *head);

/**
 * @brief Merges two sorted linked lists.
 *
 * @param a The first sorted linked list.
 * @param b The second sorted linked list.
 * @return The merged sorted linked list.
 */
ViaVerde *mergeSortedViaVerde(ViaVerde *a, ViaVerde *b);

/**
 * @brief Chronologically sorts a Via Verde linked list.
 *
 * @param head The head of the linked list.
 * @return The sorted linked list.
 */
ViaVerde *sortViaVerde(ViaVerde *head);

/**
 * @brief Calls the Via Verde sort function and updates the vehicles pointers to
 * the linked list.
 *
 * @param vehicle The vehicle to sort the movements for.
 */
void sortVehicleViaVerde(Vehicle *vehicle);
//============================================================================//

/**
 * @brief Checks if the parameters for showing vehicle movements are valid.
 *
 * @param vehicle The vehicle to show the movements for.
 * @param plate The license plate of the vehicle.
 * @return 1 if the parameters are valid, 0 otherwise.
 */
int isValidShowVehicleMovements(Vehicle *vehicle, char *plate);

/**
 * Displays the movements of a vehicle.
 *
 * @param vehicle A pointer to the Vehicle structure representing the vehicle.
 */
void showVehicleMovements(Vehicle *vehicle);

/**
 * @brief Handles the display of a vehicle's movements.
 */
void handleShowVehicleMovements();

/**
 * @brief Checks if the parameters for showing parking lot profit are valid.
 *
 * @param parkingLot The parking lot to show the profit for.
 * @param parkingLotName The name of the parking lot.
 * @param date The date to show the profit for.
 * @param dateStr The string representation of the date.
 * @return 1 if the parameters are valid, 0 otherwise.
 */
int isValidShowParkingLotProfit(ParkingLot *parkingLot, char *parkingLotName,
								Date date, char *dateStr);

//=====================MERGE SORT IMPLEMENTATION FOR LOGS=====================//
/**
 * @brief Gets the middle node of a linked list.
 *
 * @param head The head of the linked list.
 * @return The middle node of the linked list.
 */
Log *getMiddleLog(Log *head);

/**
 * @brief Merges two sorted linked lists.
 *
 * @param a The first sorted linked list.
 * @param b The second sorted linked list.
 * @return The merged sorted linked list.
 */
Log *mergeSortedLogs(Log *a, Log *b);

/**
 * @brief Chronologically sorts a Log linked list.
 *
 * @param head The head of the linked list.
 * @return The sorted linked list.
 */
Log *sortLogs(Log *head);

/**
 * @brief Calls the Log sort function and updates the parking lot pointers to
 * the linked list.
 *
 * @param parkingLot The parking lot to sort the logs for.
 */
void sortParkingLotLogs(ParkingLot *parkingLot);
//============================================================================//

/**
 * @brief Shows the profit of a parking lot for each day.
 *
 * @param parkingLot The parking lot to show the profit for.
 */
void showAllParkingLotProfit(ParkingLot *parkingLot);

/**
 * @brief Shows the profit of a parking lot on a given date.
 *
 * @param parkingLot The parking lot to show the profit for.
 * @param date The date to show the profit for.
 */
void showParkingLotProfit(ParkingLot *parkingLot, Date date);

/**
 * @brief Handles the display of a parking lot's profit.
 */
void handleShowParkingLotProfit();

/**
 * @brief Removes all logs of a vehicle containing a given parking lot.
 *
 * @param vehicle The vehicle to remove the parking lot from.
 * @param parkingLotName The name of the parking lot to be removed.
 */
void removeParkingLotFromVehicle(Vehicle *vehicle, char *parkingLotName);

/**
 * @brief Frees the memory allocated for a parking lot's logs and removes it
 * from all vehicles.
 *
 * @param parkingLot The parking lot to be freed.
 */
void freeParkingLotLogs(ParkingLot *parkingLot);

/**
 * @brief Removes a parking lot from the parking lots array.
 *
 * @param parkingLot The parking lot to be removed.
 */
void removeFromParkingLotsArray(ParkingLot *parkingLot);

/**
 * @brief Prints the remaining parking lots in the system.
 */
void printRemainingParkingLots();

/**
 * @brief Handles removing a parking lot from the system.
 */
void handleRemoveParkingLot();

/**
 * @brief Handles the command entered by the user.
 *
 * @param command The command entered by the user.
 * @return 1 if the command is EXIT, 0 otherwise.
 */
int handleCommand(char command);

/**
 * @brief Frees the memory allocated for all parking lots.
 */
void freeParkingLots();

/**
 * @brief Frees the memory allocated for all vehicles.
 */
void freeVehicles();

/**
 * @brief Frees the memory allocated for the system.
 */
void freeSystem();

/**
 * @brief The main function of the program.
 *
 * @return The exit status of the program.
 */
int main();
