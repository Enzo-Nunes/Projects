/**
 * @file date.h
 * @brief Implementation of the date data structure for this project.
 * @author ist1106336 (Enzo Nunes)
 */

#include <stdlib.h>

#define MIDNIGHT		"00:00"
#define MINUTES_IN_HOUR 60
#define HOURS_IN_DAY	24
#define DAYS_IN_YEAR	365

/**
 * @struct Date
 * @brief Represents a date and time.
 *
 * Groups five integers into a single structure representing a date and time.
 */
typedef struct Date {
	int day, month, year;
	int hour, minute;
} Date;

/**
 * Initializes a new Date object with default values. This date is initially
 * invalid.
 *
 * @return The initialized Date object.
 */
Date initDate();

/**
 * Checks if two Date objects are equal, comparing both the date and time
 * components.
 *
 * @param date1 The first Date object.
 * @param date2 The second Date object.
 * @return 1 if the Date objects are equal, 0 otherwise.
 */
int dateEquals(Date date1, Date date2);

/**
 * Checks if two Date objects have the same date component.
 *
 * @param date1 The first Date object.
 * @param date2 The second Date object.
 * @return 1 if the Date objects have the same date, 0 otherwise.
 */
int dateEqualsDate(Date date1, Date date2);

/**
 * Checks if two Date objects have the same time component.
 *
 * @param date1 The first Date object.
 * @param date2 The second Date object.
 * @return 1 if the Date objects have the same time, 0 otherwise.
 */
int dateEqualsTime(Date date1, Date date2);

/**
 * Checks if the first Date object parameter represents a date strictly before
 * the other Date object parameter.
 *
 * @param date1 The first Date object.
 * @param date2 The second Date object.
 * @return 1 if date1 is strictly before date2, 0 otherwise.
 */
int isBefore(Date date1, Date date2);

/**
 * Converts a Date object to minutes.
 *
 * @param date The Date object.
 * @return The total number of minutes passed until the date.
 */
size_t dateToMinutes(Date date);

/**
 * Calculates the time span in minutes between two Date objects.
 *
 * @param startDate The start Date object.
 * @param endDate The end Date object.
 * @return The time span in minutes.
 */
size_t getTimeSpan(Date startDate, Date endDate);

/**
 * Converts a Date object to a string representation of the date in the format
 * "dd-mm-yyyy".
 *
 * @param date The Date object.
 * @return The string representation of the date.
 */
char *getDate(Date date);

/**
 * Converts a Date object to a string representation of the time in the format
 * "hh:mm".
 *
 * @param date The Date object.
 * @return The string representation of the time.
 */
char *getTime(Date date);

/**
 * Checks if a date string has a valid format "dd-mm-yyyy".
 *
 * @param date The date string.
 * @return 1 if the date string has a valid format, 0 otherwise.
 */
int isValidDateFormat(char *date);

/**
 * Checks if a date string represents a valid date.
 *
 * @param date The date string.
 * @return 1 if the date string has a valid format, 0 otherwise.
 */
int isValidDate(char *time);

/**
 * Checks if a time string has a valid format "hh:mm" or "h:mm".
 *
 * @param time The time string.
 * @param len The length of the time string.
 * @return 1 if the time string has a valid format, 0 otherwise.
 */
int isValidTimeFormat(char *time, int len);

/**
 * Checks if a time string represents a valid time.
 *
 * @param time The time string.
 * @return 1 if the time string represents a valid time, 0 otherwise.
 */
int isValidTime(char *time);

/**
 * Checks if a parsed Date object is valid (not equal to the default initialized
 * Date object).
 *
 * @param date The parsed Date object.
 * @return 1 if the parsed Date object is valid, 0 otherwise.
 */
int isValidParsedDate(Date date);

/**
 * Parses a date and time string into a Date object.
 *
 * @param date The date string.
 * @param time The time string.
 * @return The parsed Date object. If the date or time is invalid, an
 * initialized Date object is returned.
 */
Date parseDate(char *date, char *time);