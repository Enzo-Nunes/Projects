#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 65536
#define REVERSE_FLAG "inverso"

typedef struct {
    char stop_name[STOP_NAME_SIZE];
    double lat;
    double lon;
} stop;

typedef struct {
    char line_name[LINE_NAME_SIZE];
    stop *course;
    int nr_line_stops, is_cycle;
    double total_cost, total_duration;
} line;

/* System struct for holding global values needed. */
typedef struct {
    unsigned nr_lines, nr_stops, nr_links, buffer_index;
    line *line_list;
    stop *stop_list;
} system;