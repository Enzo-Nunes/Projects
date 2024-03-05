/*
 * iaed-23 - ist1106336 - project2
 * Auxiliary source file with all secondary functions.
 */

#include "project2.h"

/*
 * Returns the next word read in the buffer.
 */
char *readNextWord(Buffer *buffer) {
    int i = 0;
    char *next_word = (char *)malloc(strlen(buffer->buffer) * sizeof(char));

    if (next_word == NULL) {
        printf(ERR_NO_MEMORY);
        exit(1);
    }

    while (buffer->buffer[buffer->index] == ' ' || buffer->buffer[buffer->index] == '\n') {
        buffer->index++;
    }

    if (buffer->buffer[buffer->index] == '"') {
        buffer->index++;
        while (buffer->buffer[buffer->index] != '"') {
            next_word[i] = buffer->buffer[buffer->index];
            i++, buffer->index++;
        }
        buffer->index++;
    } else {
        while (buffer->buffer[buffer->index] != ' ' && buffer->buffer[buffer->index] != '\n' &&
               buffer->buffer[buffer->index] != '\0') {
            next_word[i] = buffer->buffer[buffer->index];
            i++, buffer->index++;
        }
    }
    next_word[i] = '\0';

    if (i == 0) {
        free(next_word);
        return NULL;
    } else {
        return next_word;
    }
}

/*
 * Receives a list of lines and returns a new, alphabetically sorted list of
 * lines.
 */
Line *sortLines(int nr_lines, Line *lines_list) {

    int i, j, min_index;
    Line *sorted_lines_list;

    if ((sorted_lines_list = malloc(nr_lines * sizeof(Line))) == NULL) {
        printf(ERR_NO_MEMORY);
        exit(1);
    }

    memcpy(sorted_lines_list, lines_list, nr_lines * sizeof(Line));

    for (i = 0; i < nr_lines - 1; i++) {
        min_index = i;
        for (j = i + 1; j < nr_lines; j++) {
            if (strcmp(sorted_lines_list[j].name, sorted_lines_list[min_index].name) < 0) {
                min_index = j;
            }
        }
        if (min_index != i) {
            Line temp = sorted_lines_list[i];
            sorted_lines_list[i] = sorted_lines_list[min_index];
            sorted_lines_list[min_index] = temp;
        }
    }

    return sorted_lines_list;
}

/*
 * Checks if the input is an existing Line in the system. Returns the Line's
 * corresponding index in the system Line list if it is, -1 otherwise.
 */
int isLine(BusNetwork *sys, char *line_name) {
    int i;

    for (i = 0; i < sys->nr_lines; i++) {
        if (strcmp(line_name, sys->line_list[i].name) == 0) {
            return i;
        }
    }

    return NOT_FOUND;
}

/*
 * Lists all the lines in the system.
 */
void listLines(BusNetwork *sys) {
    int i;
    Line line;

    for (i = 0; i < sys->nr_lines; i++) {
        line = sys->line_list[i];
        printf("%s ", line.name);
        if (line.nr_line_stops > 0) {
            printf("%s %s ", line.origin->stop->name, line.destination->stop->name);
        }
        printf("%d %.2f %.2f\n", line.nr_line_stops, line.cost, line.duration);
    }
}

/*
 * Checks if the flag is "inverso" or any of its up to 3 character
 * abbreviations.
 */
int isReverse(char *flag) {

    int i = 0;
    char reverse[REVERSE_FLAG_SIZE] = REVERSE_FLAG;

    while (flag[i] != '\0') {
        if (flag[i] != reverse[i]) {
            return FALSE;
        }
        i++;
    }
    if (i < 3) {
        return FALSE;
    }

    return TRUE;
}

/*
 * Prints all the stops the Line passes through in order.
 */
void listLineStops(Line line, Buffer *buffer) {
    int reverse = FALSE;
    char *flag;
    StopNode *node;

    if (line.nr_line_stops == 0) {
        return;
    }

    /* Checks if the command is reversed. */
    if ((flag = readNextWord(buffer)) != NULL) {
        if ((reverse = isReverse(flag)) == FALSE) {
            free(flag);
            printf(ERR_INVALID_SORT);
            return;
        }
        free(flag);
    }

    /* Print the stops. */
    if (reverse == FALSE) {
        for (node = line.origin; node != line.destination; node = node->next) {
            printf("%s, ", node->stop->name);
        }
    } else {
        for (node = line.destination; node != line.origin; node = node->prev) {
            printf("%s, ", node->stop->name);
        }
    }
    printf("%s\n", node->stop->name);
}

/*
 * Creates a new Line with the input name and adds it to the system Line list.
 */
void createLine(BusNetwork *sys, char *line_name) {
    Line new_line;

    if (sys->nr_lines == 0) {
        sys->line_list = (Line *)malloc(CHUNK_SIZE * sizeof(Line));
    } else if (sys->nr_lines % CHUNK_SIZE == 0) {
        sys->line_list = (Line *)realloc(sys->line_list, (sys->nr_lines + CHUNK_SIZE) * sizeof(Line));
    }

    if (sys->line_list == NULL) {
        printf(ERR_NO_MEMORY);
        exit(1);
    }

    if ((new_line.name = malloc((strlen(line_name) + 1) * sizeof(char))) == NULL) {
        printf(ERR_NO_MEMORY);
        exit(1);
    }

    strcpy(new_line.name, line_name);
    new_line.nr_line_stops = 0;
    new_line.cost = 0;
    new_line.duration = 0;
    new_line.origin = NULL;
    new_line.destination = NULL;

    sys->line_list[sys->nr_lines] = new_line;
    sys->nr_lines++;
}

/*
 * Prints the coordinates of the Stop of i index in the system Stop list.
 */
void printStopCoords(BusNetwork *sys, int i) {
    printf("%16.12f %16.12f\n", sys->stop_list[i].lat, sys->stop_list[i].lon);
}

/*
 * Checks if the input is an existing Stop in the system. Returns the Stop's
 * corresponding index in the system Stop list if it is, -1 otherwise.
 */
int isStop(BusNetwork *sys, char *stop_name) {
    int i;

    for (i = 0; i < sys->nr_stops; i++) {
        if (strcmp(stop_name, sys->stop_list[i].name) == 0) {
            return i;
        }
    }

    return NOT_FOUND;
}

/*
 * Counts the number of lines that pass through the stop with the input name.
 */
int nrStopLines(BusNetwork *sys, char *stop_name) {
    StopNode *node;
    int i, count = 0;

    for (i = 0; i < sys->nr_lines; i++) {
        for (node = sys->line_list[i].origin; node != NULL; node = node->next) {
            if (strcmp(stop_name, node->stop->name) == 0) {
                count++;
                break;
            }
        }
    }
    return count;
}

/*
 * Lists all the stops in the system.
 */
void listStops(BusNetwork *sys) {
    int i;
    Stop stop;

    for (i = 0; i < sys->nr_stops; i++) {
        stop = sys->stop_list[i];
        printf("%s: %16.12f %16.12f %d\n", stop.name, stop.lat, stop.lon, nrStopLines(sys, stop.name));
    }
}

/*
 * Creates a new Stop with the input name, latitude and longitude and adds it to
 * the system Stop list.
 */
void createStop(BusNetwork *sys, char *stop_name, double lat, double lon) {
    Stop new_stop;

    if (sys->nr_stops == 0) {
        sys->stop_list = (Stop *)malloc(CHUNK_SIZE * sizeof(Stop));
    } else if (sys->nr_stops % CHUNK_SIZE == 0) {
        sys->stop_list = (Stop *)realloc(sys->stop_list, (sys->nr_stops + CHUNK_SIZE) * sizeof(Stop));
    }

    if (sys->stop_list == NULL) {
        printf(ERR_NO_MEMORY);
        exit(1);
    }

    if ((new_stop.name = malloc((strlen(stop_name) + 1) * sizeof(char))) == NULL) {
        printf(ERR_NO_MEMORY);
        exit(1);
    }
    strcpy(new_stop.name, stop_name);
    new_stop.lat = lat;
    new_stop.lon = lon;

    sys->stop_list[sys->nr_stops] = new_stop;
    sys->nr_stops++;
}

/*
 * Determines if the attempted link with the input stops is compatible with the
 * input Line.
 */
int isLinkLineCompatible(Line line, Stop origin, Stop destination) {

    if (strcmp(line.destination->stop->name, origin.name) == 0) {
        return DESTINATION_LINK;
    }

    if (strcmp(line.origin->stop->name, destination.name) == 0) {
        return ORIGIN_LINK;
    }

    printf(ERR_INVALID_LINK);
    return INVALID_LINK;
}

/*
 * Checks if the link attempted to create has valid arguments. Returns the link
 * type back to linkCommand.
 */
int isValidLink(BusNetwork *sys, int line_index, int origin_index, int destination_index, char *line_name,
                char *origin_name, char *destination_name, double cost, double duration) {

    if (line_index == NOT_FOUND) {
        printf(ERR_NO_LINE, line_name);
        return INVALID_LINK;
    }

    if (origin_index == NOT_FOUND) {
        printf(ERR_NO_STOP, origin_name);
        return INVALID_LINK;
    }

    if (destination_index == NOT_FOUND) {
        printf(ERR_NO_STOP, destination_name);
        return INVALID_LINK;
    }

    if ((cost < 0) || (duration < 0)) {
        printf(ERR_NEG_COST_OR_DURATION);
        return INVALID_LINK;
    }

    if (sys->line_list[line_index].nr_line_stops == 0) {
        return FIRST_STOPS;
    }

    return isLinkLineCompatible(sys->line_list[line_index], sys->stop_list[origin_index],
                                sys->stop_list[destination_index]);
}

/*
 * Inserts a new origin in the line of index i in the system Line list.
 */
void addNewOrigin(BusNetwork *sys, int line_index, int origin_index, double cost, double duration) {
    StopNode *origin;

    if ((origin = (StopNode *)malloc(sizeof(StopNode))) == NULL) {
        printf(ERR_NO_MEMORY);
        exit(1);
    }

    origin->stop = &sys->stop_list[origin_index];
    origin->next = sys->line_list[line_index].origin;
    origin->prev = NULL;
    origin->cost_next = cost;
    origin->duration_next = duration;

    sys->line_list[line_index].origin->prev = origin;
    sys->line_list[line_index].origin = origin;
}

/*
 * Inserts a new destination in the Line of index i in the system Line list.
 */
void addNewDestination(BusNetwork *sys, int line_index, int destination_index, double cost, double duration) {
    StopNode *destination;

    if ((destination = (StopNode *)malloc(sizeof(*destination))) == NULL) {
        printf(ERR_NO_MEMORY);
        exit(1);
    }

    destination->stop = &sys->stop_list[destination_index];
    destination->next = NULL;
    destination->prev = sys->line_list[line_index].destination;

    sys->line_list[line_index].destination->cost_next = cost;
    sys->line_list[line_index].destination->duration_next = duration;
    sys->line_list[line_index].destination->next = destination;
    sys->line_list[line_index].destination = destination;
}

/*
 * Function used to insert the first 2 stops in the Line of index i in the
 * system Line list.
 */
void addFirstStops(BusNetwork *sys, int line_index, int origin_index, int destination_index, double cost,
                   double duration) {
    StopNode *origin, *destination;

    if (((origin = (StopNode *)malloc(sizeof(StopNode))) == NULL) ||
        ((destination = (StopNode *)malloc(sizeof(StopNode))) == NULL)) {
        printf(ERR_NO_MEMORY);
        exit(1);
    }

    origin->stop = &sys->stop_list[origin_index];
    origin->next = destination;
    origin->prev = NULL;
    origin->cost_next = cost;
    origin->duration_next = duration;

    destination->stop = &sys->stop_list[destination_index];
    destination->next = NULL;
    destination->prev = origin;
    destination->cost_next = 0;
    destination->duration_next = 0;

    sys->line_list[line_index].origin = origin;
    sys->line_list[line_index].destination = destination;
    sys->line_list[line_index].nr_line_stops = 2;
    sys->line_list[line_index].cost = cost;
    sys->line_list[line_index].duration = duration;
}

/*
 * Line course-plotting function. Based on the link type, adds new stops
 * to the course of the Line of index i in the system Line list .
 */
void createLink(BusNetwork *sys, int line_index, int origin_index, int destination_index, double cost, double duration,
                int link_type) {
    switch (link_type) {
    /* Stop is inserted in the begining of the Line course. */
    case ORIGIN_LINK:
        addNewOrigin(sys, line_index, origin_index, cost, duration);
        break;

    /* Stop is inserted at the end of the Line course. */
    case DESTINATION_LINK:
        addNewDestination(sys, line_index, destination_index, cost, duration);
        break;

    /* The mentioned link actually represents the first stops to be inserted in
     * the Line course. */
    case FIRST_STOPS:
        addFirstStops(sys, line_index, origin_index, destination_index, cost, duration);
        return;
    }

    /* Updates the Line's information. */
    sys->line_list[line_index].nr_line_stops++;
    sys->line_list[line_index].cost += cost;
    sys->line_list[line_index].duration += duration;
}

/*
 * Removes a stop from a Line. Alalyzes every possible case: if the stop is the
 * only one in the Line, if it is the origin or the destination, or if it is
 * in the middle of the Line.
 */
void removeStopFromLine(BusNetwork *sys, int line_index, StopNode *node) {
    if (node->prev == NULL && node->next == NULL) {
        sys->line_list[line_index].origin = NULL;
        sys->line_list[line_index].destination = NULL;
        sys->line_list[line_index].cost = 0;
        sys->line_list[line_index].duration = 0;
    } else if (node->prev == NULL) {
        sys->line_list[line_index].origin = node->next;
        sys->line_list[line_index].cost -= node->cost_next;
        sys->line_list[line_index].duration -= node->duration_next;
        sys->line_list[line_index].origin->prev = NULL;
    } else if (node->next == NULL) {
        sys->line_list[line_index].destination = node->prev;
        sys->line_list[line_index].cost -= node->prev->cost_next;
        sys->line_list[line_index].duration -= node->prev->duration_next;
        sys->line_list[line_index].destination->next = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->prev->cost_next += node->cost_next;
        node->prev->duration_next += node->duration_next;
    }

    sys->line_list[line_index].nr_line_stops--;
    free(node);
}

/*
 * Removes a stop from a all Lines in the system.
 */
void removeStopFromLines(BusNetwork *sys, int stop_index) {
    int i;
    StopNode *node, *next;

    for (i = 0; i < sys->nr_lines; i++) {
        for (node = sys->line_list[i].origin; node != NULL; node = next) {
            next = node->next;
            if (node->stop == &sys->stop_list[stop_index]) {
                removeStopFromLine(sys, i, node);
            }
        }
    }
}

/*
 * Updates all StopNode pointers of all lines in the system after a stop is
 * removed. Essential to make sure all pointers point to the correct stop after
 * all stops to the right of the removed stop are shifted to the left.
 */
void updateLinePointers(BusNetwork *sys, int stop_index) {
    int i;
    StopNode *node;

    for (i = 0; i < sys->nr_lines; i++) {
        for (node = sys->line_list[i].origin; node != NULL; node = node->next) {
            if (node->stop > &sys->stop_list[stop_index]) {
                node->stop--;
            }
        }
    }
}

/*
 * Completely removes a stop from the system by first removing it from all the
 * lines that pass through it and then removing it from the stop list.
 */
void removeStopFromSys(BusNetwork *sys, int stop_index) {

    removeStopFromLines(sys, stop_index);

    free(sys->stop_list[stop_index].name);
    sys->nr_stops--;
    memmove(sys->stop_list + stop_index, sys->stop_list + stop_index + 1, (sys->nr_stops - stop_index) * sizeof(Stop));

    updateLinePointers(sys, stop_index);
}

/*
 * Removes a line from the system by freeing all the memory allocated for it.
 */
void removeLineFromSys(BusNetwork *sys, int line_index) {
    StopNode *node, *next;

    for (node = sys->line_list[line_index].origin; node != NULL; node = next) {
        next = node->next;
        free(node);
    }

    free(sys->line_list[line_index].name);
    sys->nr_lines--;
    memmove(sys->line_list + line_index, sys->line_list + line_index + 1, (sys->nr_lines - line_index) * sizeof(Line));
}