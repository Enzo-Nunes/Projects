/**
 * @file date.c
 * @brief Implementation of the date data structure for this project.
 * @author ist1106336 (Enzo Nunes)
 */

#include "main.h"

int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

Date initDate() {
	Date newDate;
	newDate.day = 0;
	newDate.month = 0;
	newDate.year = 0;
	newDate.hour = 0;
	newDate.minute = 0;
	return newDate;
}

int dateEquals(Date date1, Date date2) {
	return dateEqualsDate(date1, date2) && dateEqualsTime(date1, date2);
}

int dateEqualsDate(Date date1, Date date2) {
	if (date1.day != date2.day) { return FALSE; }
	if (date1.month != date2.month) { return FALSE; }
	if (date1.year != date2.year) { return FALSE; }
	return TRUE;
}

int dateEqualsTime(Date date1, Date date2) {
	if (date1.hour != date2.hour) { return FALSE; }
	if (date1.minute != date2.minute) { return FALSE; }
	return TRUE;
}

int isBefore(Date date1, Date date2) {
	if (date1.year != date2.year) { return date1.year < date2.year; }
	if (date1.month != date2.month) { return date1.month < date2.month; }
	if (date1.day != date2.day) { return date1.day < date2.day; }
	if (date1.hour != date2.hour) { return date1.hour < date2.hour; }
	return date1.minute < date2.minute;
}

size_t dateToMinutes(Date date) {
	size_t dateMins = 0;
	dateMins += date.minute;
	dateMins += date.hour * MINUTES_IN_HOUR;
	dateMins += date.day * HOURS_IN_DAY * MINUTES_IN_HOUR;
	for (int i = 0; i < date.month - 1; i++) {
		dateMins += daysInMonth[i] * HOURS_IN_DAY * MINUTES_IN_HOUR;
	}
	dateMins += date.year * DAYS_IN_YEAR * HOURS_IN_DAY * MINUTES_IN_HOUR;

	return dateMins;
}

size_t getTimeSpan(Date startDate, Date endDate) {

	size_t startDateMins = dateToMinutes(startDate);
	size_t endDateMins = dateToMinutes(endDate);

	return endDateMins - startDateMins;
}

char *getDate(Date date) {
	char *dateString = malloc(11);
	sprintf(dateString, "%02d-%02d-%04d", date.day, date.month, date.year);
	return dateString;
}

char *getTime(Date date) {
	char *timeString = malloc(6);
	sprintf(timeString, "%02d:%02d", date.hour, date.minute);
	return timeString;
}

int isValidDateFormat(char *date) {
	if (date == NULL) { return FALSE; }

	// Check if the date has the format "dd-mm-yyyy"
	if (strlen(date) != 10 || date[2] != '-' || date[5] != '-') {
		return FALSE;
	}

	// Check if the other characters are digits
	for (int i = 0; i < 10; i++) {
		if (i != 2 && i != 5 && !isdigit(date[i])) { return FALSE; }
	}

	return TRUE;
}

int isValidDate(char *date) {

	if (!isValidDateFormat(date)) { return FALSE; }

	// Extract the day, month, and year and convert them to integers
	char day[3] = {date[0], date[1], '\0'};
	char month[3] = {date[3], date[4], '\0'};
	char year[5] = {date[6], date[7], date[8], date[9], '\0'};
	int dayInt = atoi(day);
	int monthInt = atoi(month);
	int yearInt = atoi(year);

	// Check if the day, month, and year are within the valid ranges
	if (dayInt < 1 || monthInt < 1 || monthInt > 12 || yearInt < 0) {
		return FALSE;
	}

	// Check the number of days in the month
	if (dayInt > daysInMonth[monthInt - 1]) { return FALSE; }

	return TRUE;
}

int isValidTimeFormat(char *time, int len) {
	if (time == NULL) { return FALSE; }

	// Check if the time has the format "hh:mm" or "h:mm"
	if ((len != 5 && len != 4) || time[len - 3] != ':') { return FALSE; }

	// Check if the other characters are digits
	for (int i = 0; i < len; i++) {
		if (i != len - 3 && !isdigit(time[i])) { return FALSE; }
	}
	return TRUE;
}

int isValidTime(char *time) {
	int len = strlen(time);

	if (!isValidTimeFormat(time, len)) { return FALSE; }

	// Extract the hour and minute and convert them to integers
	char hour[3] = {'\0', '\0', '\0'};
	char minute[3] = {time[len - 2], time[len - 1], '\0'};
	if (len == 5) {
		hour[0] = time[0];
		hour[1] = time[1];
	} else {
		hour[0] = time[0];
	}
	int hourInt = atoi(hour);
	int minuteInt = atoi(minute);

	// Check if the hour and minute are within the valid ranges
	if (hourInt < 0 || hourInt > HOURS_IN_DAY - 1 || minuteInt < 0 ||
		minuteInt > MINUTES_IN_HOUR - 1) {
		return FALSE;
	}

	return TRUE;
}

int isValidParsedDate(Date date) {
	return date.day != 0 || date.month != 0 || date.year != 0 ||
		   date.hour != 0 || date.minute != 0;
}

Date parseDate(char *date, char *time) {
	Date parsedDate = initDate();

	if (!isValidDate(date) || !isValidTime(time)) { return parsedDate; }

	sscanf(date, "%d-%d-%d", &parsedDate.day, &parsedDate.month,
		   &parsedDate.year);
	sscanf(time, "%d:%d", &parsedDate.hour, &parsedDate.minute);
	return parsedDate;
}