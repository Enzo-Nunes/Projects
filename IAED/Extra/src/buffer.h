/**
 * @file buffer.h
 * @brief Implementation of the buffer for this project.
 * @author ist1106336 (Enzo Nunes)
 */

// Buffer size
#define BUFFER_SIZE BUFSIZ + 1

/**
 * @brief Determines the length of the next word in the input buffer.
 *
 * @return The length of the next word in the input buffer.
 */
int getNextWordLength();

/**
 * @brief Retrieves the next word from the input buffer.
 *
 * @return A pointer to the next word in the input buffer.
 */
char *getNextWord();

/**
 * @brief Reads the input from stdin and inserts it into the buffer.
 *
 * @return The first characters read from the input. Represents the command.
 */
char readInput();

/**
 * @brief Clears the input buffer.
 */
void clearBuffer();