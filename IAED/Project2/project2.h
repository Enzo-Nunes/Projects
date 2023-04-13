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
 * Error strings.
 */
#define ERR_NO_MEMORY "No memory.\n"
#define ERR_NO_STOP "%s: no such stop.\n"
#define ERR_ALREADY_STOP "%s: stop already exists.\n"
#define ERR_NO_LINE "%s: no such line.\n"
#define ERR_INVAL_SORT "incorrect sort option.\n"
#define ERR_INVALID_LINK "link cannot be associated with bus line.\n"
#define ERR_NEG_COST_OR_DURATION "negative cost or duration.\n"

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

/*
 * Buffer function.
 */
char *readNextWord(Buffer *buffer);

/*
 * Line functions.
 */
Line *sortLines(int nr_lines, Line *lines_list);
int isLine(BusNetwork *sys, char *line_name);
void listLines(BusNetwork *sys);
int isReverse(char *flag);
void listLineStops(Line line, Buffer *buffer);
void createLine(BusNetwork *sys, char *line_name);

/*
 * Stop functions.
 */
void printStopCoords(BusNetwork *sys, int i);
int isStop(BusNetwork *sys, char *stop_name);
int nrStopLines(BusNetwork *sys, char *stop_name);
void listStops(BusNetwork *sys);
void createStop(BusNetwork *sys, char *stop_name, double lat, double lon);

/*
 * Link functions.
 */
int isLinkLineCompatible(Line line, Stop origin, Stop destination);
int isValidLink(BusNetwork *sys, int line_index, int origin_index,
                int destination_index, char *line_name, char *origin_name,
                char *destination_name, double cost, double duration);
void addNewOrigin(BusNetwork *sys, int line_index, int origin_index,
                  double cost, double duration);
void addNewDestination(BusNetwork *sys, int line_index, int destination_index,
                       double cost, double duration);
void addFirstStops(BusNetwork *sys, int line_index, int origin_index,
                   int destination_index, double cost, double duration);
void createLink(BusNetwork *sys, int line_index, int origin_index,
                int destination_index, double cost, double duration,
                int link_type);

/*
 * Remove functions.
 */
void removeStopFromLine(BusNetwork *sys, int line_index, StopNode *node);
void removeStopFromLines(BusNetwork *sys, int stop_index);
void updateLinePointers(BusNetwork *sys, int stop_index);
void removeStopFromSys(BusNetwork *sys, int stop_index);
void removeLineFromSys(BusNetwork *sys, int line_index);