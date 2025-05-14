#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <errno.h>	  // Error number definitions
#include <fcntl.h>	  // File control options
#include <limits.h>	  // Implementation-defined constants
#include <pthread.h>  // POSIX threads
#include <signal.h>	  // Signal handling
#include <stddef.h>	  // Standard library definitions
#include <stdio.h>	  // Standard input/output functions
#include <stdlib.h>	  // Standard library functions
#include <string.h>	  // String manipulation functions
#include <sys/stat.h> // File information
#include <time.h>	  // Time/date utilities
#include <unistd.h>	  // Standard symbolic constants and types

#define MAX_RESERVATION_SIZE 256
#define STATE_ACCESS_DELAY_US 500000
#define MAX_JOB_FILE_NAME_SIZE 256
#define MAX_SESSION_COUNT 8
#define MAX_PIPENAME_SIZE 40
#define TO_CHAR(x) (x + '0')

// Operation Codes
enum OP_CODE {
	OP_SETUP = 1,
	OP_QUIT = 2,
	OP_CREATE = 3,
	OP_RESERVE = 4,
	OP_SHOW = 5,
	OP_LIST = 6,
};

#endif // CONSTANTS_H