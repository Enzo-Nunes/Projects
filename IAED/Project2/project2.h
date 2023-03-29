/* iaed-23 - ist1106336 - project2 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 65536
#define REVERSE_FLAG "inverso"
#define CHUNK_SIZE 16

typedef struct Stop {
    char *stop_name;
    int nr_lines;
    double lat, lon;
} Stop;

typedef struct Line {
    char *line_name;
    int *course;
    int nr_line_stops;
    int is_cycle;
    double cost, duration;
} Line;

/* System struct for holding global values needed. */
typedef struct BusNetwork {
    int nr_lines, nr_stops, buffer_index;
    Line *line_list;
    Stop *stop_list;
} BusNetwork;