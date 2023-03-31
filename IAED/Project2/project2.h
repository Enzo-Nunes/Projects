/* iaed-23 - ist1106336 - project2 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 65536
#define REVERSE_FLAG "inverso"
#define CHUNK_SIZE 32

typedef struct Stop {
    char *name;
    double lat, lon;
} Stop;

typedef struct StopNode {
    Stop *stop;
    int cost_next, duration_next;
    struct StopNode *next;
    struct StopNode *prev;
} StopNode;

typedef struct Line {
    char *line_name;
    StopNode *origin;
    StopNode *destination;
    int nr_line_stops;
    double cost, duration;
} Line;

typedef struct BusNetwork {
    int nr_lines, nr_stops, buffer_index;
    Line *line_list;
    Stop *stop_list;
} BusNetwork;