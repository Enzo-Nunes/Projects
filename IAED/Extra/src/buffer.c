/**
 * @file buffer.c
 * @brief Implementation of the buffer for this project.
 * @author ist1106336 (Enzo Nunes)
 */

#include "main.h"

// Buffer and its current index.
char buffer[BUFFER_SIZE];
int buffer_index = 0;

int getNextWordLength() {
	int word_length = 0, length_index = buffer_index;

	if (buffer[length_index] == '"') {
		length_index++;
		while (buffer[length_index] != '"') { word_length++, length_index++; }
		length_index++;
	} else {
		while (buffer[length_index] != ' ' && buffer[length_index] != '\n') {
			word_length++, length_index++;
		}
	}
	return word_length;
}

char *getNextWord() {
	int i = 0;

	// Skip leading spaces
	while (buffer[buffer_index] == ' ' || buffer[buffer_index] == '\t') {
		buffer_index++;
	}

	int word_length = getNextWordLength();

	// Allocate memory for the next word
	char *next_word = malloc(word_length + 1); // +1 for the null character

	// Second pass: copy the characters of the next word
	if (buffer[buffer_index] == '"') {
		buffer_index++;
		while (buffer[buffer_index] != '"') {
			next_word[i] = buffer[buffer_index];
			i++, buffer_index++;
		}
		buffer_index++;
	} else {
		while (buffer[buffer_index] != ' ' && buffer[buffer_index] != '\n') {
			next_word[i] = buffer[buffer_index];
			i++, buffer_index++;
		}
	}

	next_word[i] = '\0';

	if (i == 0) {
		free(next_word);
		return NULL;
	} else {
		return next_word;
	}
}

char readInput() {
	char c, command;
	// Skip leading spaces
	while ((command = getchar()) == ' ' || command == '\n' || command == '\t') {
	}
	do {
		c = getchar();
		buffer[buffer_index++] = c;
	} while (c != '\n');
	buffer_index = 0;
	return command;
}

void clearBuffer() {
	memset(buffer, 0, BUFFER_SIZE);
	buffer_index = 0;
}