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

/*
 * Link types.
 */
#define INVALID_LINK -1
#define ORIGIN_LINK 0
#define DESTINATION_LINK 1
#define FIRST_STOPS 2

/*
 * Stop structure.
 */
typedef struct Stop {
    char *name;
    double lat, lon;
} Stop;

/*
 * StopNode structure. Used in the line structure as a doubly linked list of
 * stops to represent it's course.
 */
typedef struct StopNode {
    Stop *stop;
    double cost_next, duration_next;
    struct StopNode *next;
    struct StopNode *prev;
} StopNode;

/*
 * Line structure. Has pointers to the origin and destination of it's course,
 * represented by a doubly linked list of stops.
 */
typedef struct Line {
    char *name;
    StopNode *origin;
    StopNode *destination;
    int nr_line_stops;
    double cost, duration;
} Line;

/*
 * BusNetwork system structure. 'Brain' of the program.
 */
typedef struct BusNetwork {
    int nr_lines, nr_stops;
    Line *line_list;
    Stop *stop_list;
} BusNetwork;

/*
 * Buffer structure. Used to store the user input contents.
 */
typedef struct Buffer {
    char *buffer;
    int index;
} Buffer;