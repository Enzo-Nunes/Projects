#include "constants.h"
#include "libraries.h"
#include "operations.h"
#include "parser.h"

/**
 * Function that represents the thread execution for a process.
 * @param arg The argument passed to the thread.
 * @return The return value of the thread.
 */
void *processThread(void *arg);

/**
 * Function that processes a file.
 * @param name The name of the file to be processed.
 * @param MAX_THREADS The maximum number of threads to be used.
 */
void processFile(char *name, const int MAX_THREADS);