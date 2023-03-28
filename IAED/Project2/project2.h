#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 65536
#define REVERSE_FLAG "inverso"
#define CHUNK_SIZE 16

typedef struct {
    char *stop_name;
    double lat;
    double lon;
} stop;

typedef struct {
    char *line_name;
    unsigned *course;
    unsigned nr_line_stops;
    int is_cycle;
    double total_cost, total_duration;
} line;

/* System struct for holding global values needed. */
typedef struct {
    unsigned nr_lines, nr_stops, nr_links, buffer_index;
    line *line_list;
    stop *stop_list;

} system;