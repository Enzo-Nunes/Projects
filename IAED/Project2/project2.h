/* iaed-23 - ist1106336 - project2 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 65536
#define REVERSE_FLAG "inverso"
#define REVERSE_FLAG_SIZE 8
#define CHUNK_SIZE 64

#define TRUE 1
#define FALSE 0
#define NOT_FOUND -1

/* Link types. */
#define INVALID_LINK -1
#define ORIGIN_LINK 0
#define DESTINATION_LINK 1
#define FIRST_STOPS 2

/*
 * Stop structure. Defined by name and coordinates only.
 */
typedef struct Stop {
    char *name;
    double lat, lon;
} Stop;

/*
 * StopNode structure. Defined by a pointer to a Stop, the cost and duration
 * to the next stop, and pointers to the next and previous nodes.
 */
typedef struct StopNode {
    Stop *stop;
    double cost_next, duration_next;
    struct StopNode *next;
    struct StopNode *prev;
} StopNode;

/*
 * Line structure. Defined by name, pointers to origin and destination stop
 * nodes, the number of stops in the line, and the total cost and duration of
 * the line.
 */
typedef struct Line {
    char *name;
    StopNode *origin;
    StopNode *destination;
    int nr_line_stops;
    double cost, duration;
} Line;

/*
 * BusNetwork system structure. Defined by the number of lines and stops, a
 * buffer index for the readNextWord function, and pointers to the line and stop
 * lists.
 */
typedef struct BusNetwork {
    int nr_lines, nr_stops, buffer_index;
    Line *line_list;
    Stop *stop_list;
} BusNetwork;