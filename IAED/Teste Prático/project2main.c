/*
 * iaed-23 - ist1106336 - project2
 * Main source file with main functions.
 */

#include "project2.h"

/*
 * Manages all the Line commands. Checks all argument options and validity, and
 * calls the corresponding functions.
 */
void lineCommand(BusNetwork *sys, Buffer *buffer) {

    char *line_name;
    int i;

    if ((line_name = readNextWord(buffer)) == NULL) {
        listLines(sys);
    } else {
        if ((i = isLine(sys, line_name)) == NOT_FOUND) {
            createLine(sys, line_name);
        } else {
            listLineStops(sys->line_list[i], buffer);
        }
    }
    free(line_name);
}

/*
 * Manages all Stop commands. Checks all argument options and validity, and
 * calls the corresponding functions.
 */
void stopCommand(BusNetwork *sys, Buffer *buffer) {

    char *stop_name, *lat, *lon;
    int stop_index;

    if ((stop_name = readNextWord(buffer)) == NULL) {
        listStops(sys);
    } else {
        if ((lat = readNextWord(buffer)) != NULL) {
            if (isStop(sys, stop_name) == NOT_FOUND) {
                lon = readNextWord(buffer);
                createStop(sys, stop_name, atof(lat), atof(lon));
                free(lon);
            } else {
                printf(ERR_ALREADY_STOP, stop_name);
                free(lat);
                free(stop_name);
                return;
            }
            free(lat);
        } else {
            if ((stop_index = isStop(sys, stop_name)) != NOT_FOUND) {
                printStopCoords(sys, stop_index);
            } else {
                printf(ERR_NO_STOP, stop_name);
                free(stop_name);
                return;
            }
        }
        free(stop_name);
    }
}

/*
 * Manages all link commands. Analyzes the link type and adds stops to the Line
 * accordingly.
 */
void linkCommand(BusNetwork *sys, Buffer *buffer) {

    int line_index, origin_index, destination_index, link_type;
    double cost, duration;
    char *line_name, *origin_name, *destination_name, *cost_pre, *duration_pre;

    line_name = readNextWord(buffer);
    origin_name = readNextWord(buffer);
    destination_name = readNextWord(buffer);
    cost_pre = readNextWord(buffer);
    duration_pre = readNextWord(buffer);

    line_index = isLine(sys, line_name);
    origin_index = isStop(sys, origin_name);
    destination_index = isStop(sys, destination_name);
    cost = atof(cost_pre);
    duration = atof(duration_pre);

    if ((link_type = isValidLink(sys, line_index, origin_index, destination_index, line_name, origin_name,
                                 destination_name, cost, duration)) != NOT_FOUND) {
        createLink(sys, line_index, origin_index, destination_index, cost, duration, link_type);
    }
    free(line_name);
    free(origin_name);
    free(destination_name);
    free(cost_pre);
    free(duration_pre);
}

void findLines(BusNetwork *sys, int stop_index) {
    int i, count = 0;

    for (i = 0; i < sys->nr_lines; i++) {
        if (sys->line_list[i].destination != NULL &&
            strcmp(sys->line_list[i].destination->stop->name, sys->stop_list[stop_index].name) == 0) {
            if (count != 0) {
                printf(" ");
            }
            printf("%s", sys->line_list[i].name);
            count++;
        }
    }
    if (count != 0) {
        printf("\n");
    }
}

void findLineCommand(BusNetwork *sys, Buffer *buffer) {
    char *stop_name;
    int stop_index;

    stop_name = readNextWord(buffer);
    stop_index = isStop(sys, stop_name);

    if (stop_index == NOT_FOUND) {
        printf(ERR_NO_STOP, stop_name);
    } else {
        findLines(sys, stop_index);
    }
    free(stop_name);
}

/*
 * Lists all the Line intersections in the system.
 */
void intsecCommand(BusNetwork *sys) {
    int i, j, nr_stop_lines;
    Line *sorted_lines_list = sortLines(sys->nr_lines, sys->line_list);
    StopNode *node;

    for (i = 0; i < sys->nr_stops; i++) {
        if ((nr_stop_lines = nrStopLines(sys, sys->stop_list[i].name)) > 1) {
            printf("%s %d:", sys->stop_list[i].name, nr_stop_lines);
            for (j = 0; j < sys->nr_lines; j++) {
                for (node = sorted_lines_list[j].origin; node != NULL; node = node->next) {
                    if (node->stop == &sys->stop_list[i]) {
                        printf(" %s", sorted_lines_list[j].name);
                        break;
                    }
                }
            }
            printf("\n");
        }
    }
    free(sorted_lines_list);
}

/*
 * Manages removing stops from the system. Prints an error message if the stop
 * does not exist.
 */
void removeStopCommand(BusNetwork *sys, Buffer *buffer) {
    char *stop_name;
    int stop_index;

    stop_name = readNextWord(buffer);
    stop_index = isStop(sys, stop_name);

    if (stop_index == NOT_FOUND) {
        printf(ERR_NO_STOP, stop_name);
    } else {
        removeStopFromSys(sys, stop_index);
    }
    free(stop_name);
}

/*
 * Manages removing lines from the system. Prints an error message if the stop
 * does not exist.
 */
void removeLineCommand(BusNetwork *sys, Buffer *buffer) {
    char *line_name;
    int line_index;

    line_name = readNextWord(buffer);
    line_index = isLine(sys, line_name);

    if (line_index == NOT_FOUND) {
        printf(ERR_NO_LINE, line_name);
    } else {
        removeLineFromSys(sys, line_index);
    }
    free(line_name);
}

/*
 *  Resets the system by freeing all memory allocated to it.
 */
void freeSystem(BusNetwork *sys) {
    int i;
    StopNode *node, *next;

    for (i = 0; i < sys->nr_lines; i++) {
        for (node = sys->line_list[i].origin; node != NULL; node = next) {
            next = node->next;
            free(node);
        }
    }

    for (i = 0; i < sys->nr_stops; i++) {
        free(sys->stop_list[i].name);
    }
    if (sys->stop_list != NULL) {
        free(sys->stop_list);
    }

    for (i = 0; i < sys->nr_lines; i++) {
        free(sys->line_list[i].name);
    }
    if (sys->line_list != NULL) {
        free(sys->line_list);
    }

    free(sys);
}

/*
 * Starts a new BusNetwork system and returns a pointer to it.
 */
BusNetwork *startSystem() {
    BusNetwork *sys = (BusNetwork *)malloc(sizeof(BusNetwork));

    sys->nr_lines = 0;
    sys->nr_stops = 0;

    /* Memory is allocated later when adding first lines or stops. */
    sys->line_list = NULL;
    sys->stop_list = NULL;

    return sys;
}

/*
 * Reads the input from the user and inserts it into the buffer.
 */
Buffer *getBuffer(Buffer *buffer) {
    char c;
    int count;

    memset(buffer->buffer, 0, BUFFER_SIZE);
    buffer->index = 0;

    for (count = 0; (c = getchar()) != '\n'; count++) {
        buffer->buffer[count] = c;
    }

    return buffer;
}

/*
 * Main function. Inserts the input into the buffer and calls all the other main
 * command functions. Inputs are asked indefinitely until the user inputs 'q'.
 */
int main() {
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));
    BusNetwork *sys;
    buffer->buffer = (char *)malloc(BUFFER_SIZE);

    sys = startSystem();
    while (TRUE) {
        buffer = getBuffer(buffer);

        switch (buffer->buffer[0]) {
        case 'l':
            buffer->index = 2;
            linkCommand(sys, buffer);
            break;
        case 'p':
            buffer->index = 2;
            stopCommand(sys, buffer);
            break;
        case 'c':
            buffer->index = 2;
            lineCommand(sys, buffer);
            break;
        case 'e':
            buffer->index = 2;
            removeStopCommand(sys, buffer);
            break;
        case 'r':
            buffer->index = 2;
            removeLineCommand(sys, buffer);
            break;
        case 'f':
            buffer->index = 2;
            findLineCommand(sys, buffer);
            break;
        case 'i':
            intsecCommand(sys);
            break;
        case 'a':
            freeSystem(sys);
            sys = startSystem();
            break;
        case 'q':
            freeSystem(sys);
            free(buffer->buffer);
            free(buffer);
            return 0;
        }
    }
}