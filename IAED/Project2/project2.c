/* iaed-23 - ist1106336 - project2 */

#include "project2.h"

/*
 * Returns the next word read in the buffer.
 */
char *readNextWord(BusNetwork *sys, char buffer[]) {
    int i = 0;
    char *next_word = (char *)malloc(strlen(buffer) * sizeof(char));

    if (next_word == NULL) {
        printf("No memory.\n");
        exit(1);
    }

    while (buffer[sys->buffer_index] == ' ' ||
           buffer[sys->buffer_index] == '\n') {
        sys->buffer_index++;
    }

    if (buffer[sys->buffer_index] == '"') {
        sys->buffer_index++;
        while (buffer[sys->buffer_index] != '"') {
            next_word[i] = buffer[sys->buffer_index];
            i++, sys->buffer_index++;
        }
        sys->buffer_index++;
    } else {
        while (buffer[sys->buffer_index] != ' ' &&
               buffer[sys->buffer_index] != '\n' &&
               buffer[sys->buffer_index] != '\0') {
            next_word[i] = buffer[sys->buffer_index];
            i++, sys->buffer_index++;
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
Line *sortLines(int nr_lines, Line lines_list[]) {

    int i, j, min_index;
    Line *sorted_lines_list;

    if ((sorted_lines_list = malloc(nr_lines * sizeof(Line))) == NULL) {
        printf("No memory.\n");
        exit(1);
    }

    memcpy(sorted_lines_list, lines_list, nr_lines * sizeof(Line));

    for (i = 0; i < nr_lines - 1; i++) {
        min_index = i;
        for (j = i + 1; j < nr_lines; j++) {
            if (strcmp(sorted_lines_list[j].name,
                       sorted_lines_list[min_index].name) < 0) {
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
int isLine(BusNetwork *sys, char line_name[]) {
    int i;

    for (i = 0; i < sys->nr_lines; i++) {
        if (strcmp(line_name, sys->line_list[i].name) == 0) {
            return i;
        }
    }

    return -1;
}

/*
 * List all the lines in the system.
 */
void listLines(BusNetwork *sys) {
    int i;
    Line line;

    for (i = 0; i < sys->nr_lines; i++) {
        line = sys->line_list[i];
        printf("%s ", line.name);
        if (line.nr_line_stops > 0) {
            printf("%s %s ", line.origin->stop->name,
                   line.destination->stop->name);
        }
        printf("%d %.2f %.2f\n", line.nr_line_stops, line.cost, line.duration);
    }
}

/*
 * Checks if the flag is "inverso" or any of its up to 3 character
 * abbreviations. Returns 0 if it is, 1 otherwise.
 */
int isReverse(char flag[]) {

    int i = 0;
    char reverse[8] = REVERSE_FLAG;

    while (flag[i] != '\0') {
        if (flag[i] != reverse[i]) {
            return 1;
        }
        i++;
    }
    if (i < 3) {
        return 1;
    }

    return 0;
}

/*
 * Prints all the stops in the line corresponding to the index i in the system
 * line list.
 */
void listLineStops(BusNetwork *sys, Line line, char buffer[]) {
    int reverse = 0;
    char *flag;
    StopNode *node;

    if (line.nr_line_stops == 0) {
        return;
    }

    /* Checks if the command is reversed. */
    if ((flag = readNextWord(sys, buffer)) != NULL) {
        if (isReverse(flag) == 0) {
            free(flag);
            reverse = 1;
        } else {
            printf("incorrect sort option.\n");
            return;
        }
    }

    /* Print the stops. */
    if (reverse == 0) {
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
        sys->line_list = (Line *)realloc(
            sys->line_list, (sys->nr_lines + CHUNK_SIZE) * sizeof(Line));
    }

    if (sys->line_list == NULL) {
        printf("No memory.\n");
        exit(1);
    }

    if ((new_line.name = malloc((strlen(line_name) + 1) * sizeof(char))) ==
        NULL) {
        printf("No memory.\n");
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
 * Main function that manages all the Line commands.
 */
void lineCommand(BusNetwork *sys, char buffer[]) {

    char *line_name;
    int i;

    if ((line_name = readNextWord(sys, buffer)) == NULL) {
        listLines(sys);
    } else {
        if ((i = isLine(sys, line_name)) != -1) {
            listLineStops(sys, sys->line_list[i], buffer);
        } else {
            createLine(sys, line_name);
        }
    }
    free(line_name);
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
int isStop(BusNetwork *sys, char stop_name[]) {
    int i;

    for (i = 0; i < sys->nr_stops; i++) {
        if (strcmp(stop_name, sys->stop_list[i].name) == 0) {
            return i;
        }
    }

    return -1;
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
        printf("%s: %16.12f %16.12f %d\n", stop.name, stop.lat, stop.lon,
               nrStopLines(sys, stop.name));
    }
}

/*
 * Creates a new Stop with the input name, latitude and longitude and adds it to
 * the system Stop list.
 */
void createStop(BusNetwork *sys, char *stop_name, char *lat, char *lon) {
    Stop new_stop;

    if (sys->nr_stops == 0) {
        sys->stop_list = (Stop *)malloc(CHUNK_SIZE * sizeof(Stop));
    } else if (sys->nr_stops % CHUNK_SIZE == 0) {
        sys->stop_list = (Stop *)realloc(
            sys->stop_list, (sys->nr_stops + CHUNK_SIZE) * sizeof(Stop));
    }

    if (sys->stop_list == NULL) {
        printf("No memory.\n");
        exit(1);
    }

    if ((new_stop.name = malloc((strlen(stop_name) + 1) * sizeof(char))) ==
        NULL) {
        printf("No memory.\n");
        exit(1);
    }
    strcpy(new_stop.name, stop_name);
    new_stop.lat = atof(lat);
    new_stop.lon = atof(lon);

    sys->stop_list[sys->nr_stops] = new_stop;
    sys->nr_stops++;
}

/*
 * Main function that manages all Stop commands, i.e. creating stops, listing
 * all stops and printing specific Stop coordinates.
 */
void stopCommand(BusNetwork *sys, char buffer[]) {

    char *stop_name, *lat, *lon;
    int stop_index;

    if ((stop_name = readNextWord(sys, buffer)) == NULL) {
        listStops(sys);
    } else {
        if ((lat = readNextWord(sys, buffer)) != NULL) {
            if (isStop(sys, stop_name) == -1) {
                lon = readNextWord(sys, buffer);
                createStop(sys, stop_name, lat, lon);
                free(lon);
            } else {
                printf("%s: stop already exists.\n", stop_name);
                free(lat);
                free(stop_name);
                return;
            }
            free(lat);
        } else {
            if ((stop_index = isStop(sys, stop_name)) != -1) {
                printStopCoords(sys, stop_index);
            } else {
                printf("%s: no such stop.\n", stop_name);
                free(stop_name);
                return;
            }
        }
        free(stop_name);
    }
}

/*
 * Function called only by isValidLink that checks whether the link attempted to
 * create is valid for the given Line. Returns integers that represent link
 * types back to isValidLink.
 */
int isLinkLineCompatible(Line line, Stop origin, Stop destination) {

    if (strcmp(line.destination->stop->name, origin.name) == 0) {
        return 1;
    }

    if (strcmp(line.origin->stop->name, destination.name) == 0) {
        return 0;
    }

    printf("link cannot be associated with bus line.\n");
    return -1;
}

/*
 * Checks if the link attempted to create has valid arguments. Returns -1 if
 * it's invalid, 0 if it's an origin link, 1 if it's a destination link and 2 if
 * these are the first stops to be inserted in the Line course.
 */
int isValidLink(BusNetwork *sys, int line_index, int origin_index,
                int destination_index, char *line_name, char *origin_name,
                char *destination_name, double cost, double duration) {

    if (line_index == -1) {
        printf("%s: no such line.\n", line_name);
        return -1;
    }

    if (origin_index == -1) {
        printf("%s: no such stop.\n", origin_name);
        return -1;
    }

    if (destination_index == -1) {
        printf("%s: no such stop.\n", destination_name);
        return -1;
    }

    if ((cost < 0) || (duration < 0)) {
        printf("negative cost or duration.\n");
        return -1;
    }

    if (sys->line_list[line_index].nr_line_stops == 0) {
        return 2;
    }

    return isLinkLineCompatible(sys->line_list[line_index],
                                sys->stop_list[origin_index],
                                sys->stop_list[destination_index]);
}

void addNewOrigin(BusNetwork *sys, int line_index, int origin_index,
                  double cost, double duration) {
    StopNode *origin;

    if ((origin = (StopNode *)malloc(sizeof(StopNode))) == NULL) {
        printf("No memory.\n");
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

void addNewDestination(BusNetwork *sys, int line_index, int destination_index,
                       double cost, double duration) {
    StopNode *destination;

    if ((destination = (StopNode *)malloc(sizeof(*destination))) == NULL) {
        printf("No memory.\n");
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
void addFirstStops(BusNetwork *sys, int line_index, int origin_index,
                   int destination_index, double cost, double duration) {
    StopNode *origin, *destination;

    if (((origin = (StopNode *)malloc(sizeof(StopNode))) == NULL) ||
        ((destination = (StopNode *)malloc(sizeof(StopNode))) == NULL)) {
        printf("No memory.\n");
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
void createLink(BusNetwork *sys, int line_index, int origin_index,
                int destination_index, double cost, double duration,
                int link_type) {
    switch (link_type) {
    /* Origin link. Stop is inserted in the begining of the Line course. */
    case 0:
        addNewOrigin(sys, line_index, origin_index, cost, duration);
        break;

    /* Destination link. Stop is inserted at the end of the Line course. */
    case 1:
        addNewDestination(sys, line_index, destination_index, cost, duration);
        break;

    /* First stops. The mentioned link actually represents the first stops to be
     * inserted in the Line course. */
    case 2:
        addFirstStops(sys, line_index, origin_index, destination_index, cost,
                      duration);
        return;
    }

    /* Updates the Line's information. */
    sys->line_list[line_index].nr_line_stops++;
    sys->line_list[line_index].cost += cost;
    sys->line_list[line_index].duration += duration;
}

/*
 * Main link function that manages all link commands. Analyzes the link type and
 * adds stops to the Line accordingly.
 */
void linkCommand(BusNetwork *sys, char buffer[]) {

    int line_index, origin_index, destination_index, link_type;
    double cost, duration;
    char *line_name, *origin_name, *destination_name, *cost_pre, *duration_pre;

    line_name = readNextWord(sys, buffer);
    origin_name = readNextWord(sys, buffer);
    destination_name = readNextWord(sys, buffer);
    cost_pre = readNextWord(sys, buffer);
    duration_pre = readNextWord(sys, buffer);

    line_index = isLine(sys, line_name);
    origin_index = isStop(sys, origin_name);
    destination_index = isStop(sys, destination_name);
    cost = atof(cost_pre);
    duration = atof(duration_pre);

    if ((link_type = isValidLink(sys, line_index, origin_index,
                                 destination_index, line_name, origin_name,
                                 destination_name, cost, duration)) != -1) {
        createLink(sys, line_index, origin_index, destination_index, cost,
                   duration, link_type);
    }
    free(line_name);
    free(origin_name);
    free(destination_name);
    free(cost_pre);
    free(duration_pre);
}

/*
 * Intersection Command. Lists all the Line intersections in the system.
 */
void intsecCommand(BusNetwork *sys) {
    int i, j, nr_stop_lines;
    Line *sorted_lines_list = sortLines(sys->nr_lines, sys->line_list);
    StopNode *node;

    for (i = 0; i < sys->nr_stops; i++) {
        if ((nr_stop_lines = nrStopLines(sys, sys->stop_list[i].name)) > 1) {
            printf("%s %d:", sys->stop_list[i].name, nr_stop_lines);
            for (j = 0; j < sys->nr_lines; j++) {
                for (node = sorted_lines_list[j].origin; node != NULL;
                     node = node->next) {
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

void removeStopFromLine(BusNetwork *sys, int line_index, StopNode *node) {
    if (node->prev == NULL) {
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

void removeStopFromLines(BusNetwork *sys, int stop_index) {
    int i;
    StopNode *node;

    for (i = 0; i < sys->nr_lines; i++) {
        for (node = sys->line_list[i].origin; node != NULL; node = node->next) {
            if (node->stop == &sys->stop_list[stop_index]) {
                removeStopFromLine(sys, i, node);
            }
        }
    }
}

void updateCoursePointers(BusNetwork *sys, int stop_index) {
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

void removeStopFromSys(BusNetwork *sys, int stop_index) {

    removeStopFromLines(sys, stop_index);

    free(sys->stop_list[stop_index].name);
    sys->nr_stops--;
    memmove(sys->stop_list + stop_index, sys->stop_list + stop_index + 1,
            (sys->nr_stops - stop_index) * sizeof(Stop));

    updateCoursePointers(sys, stop_index);

    sys->stop_list =
        (Stop *)realloc(sys->stop_list, sys->nr_stops * sizeof(Stop));
}

void removeStopCommand(BusNetwork *sys, char buffer[]) {
    char *stop_name;
    int stop_index;

    stop_name = readNextWord(sys, buffer);
    stop_index = isStop(sys, stop_name);

    if (stop_index != -1) {
        removeStopFromSys(sys, stop_index);
    } else {
        printf("%s: no such stop.\n", stop_name);
    }
    free(stop_name);
}

void removeLineFromSys(BusNetwork *sys, int line_index) {
    StopNode *node, *next;

    for (node = sys->line_list[line_index].origin; node != NULL; node = next) {
        next = node->next;
        free(node);
    }

    free(sys->line_list[line_index].name);
    sys->nr_lines--;
    memmove(sys->line_list + line_index, sys->line_list + line_index + 1,
            (sys->nr_lines - line_index) * sizeof(Line));

    sys->line_list =
        (Line *)realloc(sys->line_list, sys->nr_lines * sizeof(Line));
}

void removeLineCommand(BusNetwork *sys, char buffer[]) {
    char *line_name;
    int line_index;

    line_name = readNextWord(sys, buffer);
    line_index = isLine(sys, line_name);

    if (line_index != -1) {
        removeLineFromSys(sys, line_index);
    } else {
        printf("%s: no such line.\n", line_name);
    }
    free(line_name);
}

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
    if (sys->stop_list != NULL)
        free(sys->stop_list);

    for (i = 0; i < sys->nr_lines; i++) {
        free(sys->line_list[i].name);
    }
    if (sys->line_list != NULL)
        free(sys->line_list);

    free(sys);
}

BusNetwork *startSystem() {
    BusNetwork *sys;

    if ((sys = (BusNetwork *)malloc(sizeof(BusNetwork))) == NULL) {
        printf("No memory.\n");
        exit(1);
    }

    sys->buffer_index = 0;
    sys->nr_lines = 0;
    sys->nr_stops = 0;

    /* Memory is allocated to the arrays later when adding lines or stops. */
    sys->line_list = NULL;
    sys->stop_list = NULL;

    return sys;
}

char *getBuffer(BusNetwork *sys) {
    char c;
    char *buffer;
    int count;

    if ((buffer = (char *)malloc(BUFFER_SIZE * sizeof(char))) == NULL) {
        printf("No memory.\n");
        exit(1);
    }

    memset(buffer, 0, BUFFER_SIZE);
    sys->buffer_index = 0;

    for (count = 0; (c = getchar()) != '\n'; count++) {
        buffer[count] = c;
    }

    return buffer;
}

/*
 * Main function. Inserts the input into the buffer and calls all the other main
 * command functions. Inputs are asked indefinitely until the user inputs 'q'.
 */
int main() {

    char *buffer;
    BusNetwork *main_sys;

    main_sys = startSystem();
    while (1) {
        buffer = getBuffer(main_sys);

        switch (buffer[0]) {
        case 'l':
            linkCommand(main_sys, buffer + 2);
            break;
        case 'p':
            stopCommand(main_sys, buffer + 2);
            break;
        case 'c':
            lineCommand(main_sys, buffer + 2);
            break;
        case 'i':
            intsecCommand(main_sys);
            break;
        case 'e':
            removeStopCommand(main_sys, buffer + 2);
            break;
        case 'r':
            removeLineCommand(main_sys, buffer + 2);
            break;
        case 'a':
            freeSystem(main_sys);
            main_sys = startSystem();
            break;
        case 'q':
            freeSystem(main_sys);
            free(buffer);
            return 0;
        }
        free(buffer);
    }
}