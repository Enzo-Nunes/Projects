/* iaed-23 - ist1106336 - project1 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE BUFSIZ + 1
#define STOP_NAME_SIZE 51
#define LINE_NAME_SIZE 21
#define MAX_STOPS 10000
#define MAX_LINES 200
#define MAX_LINKS 30000
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

/* Global lists of lines and stops. */
line line_list[MAX_LINES];
stop stop_list[MAX_STOPS];

/* Global variable used to keep track of the next word to read in the buffer
   when calling the readNextWork(buffer) function. Resets back to 0 every time
   a new input is asked and the buffer is created again. */
int buffer_index = 0;

/* Global variables to keep track of the numbers of lines and stops. */
int nr_lines = 0;
int nr_stops = 0;

/*
 * Function that reads the next word in the buffer and returns it.
 */
char *readNextWord(char buffer[]) {
    int i = 0;
    char *next_word = malloc(BUFSIZ);

    while (buffer[buffer_index] == ' ' || buffer[buffer_index] == '\n') {
        buffer_index++;
    }

    if (buffer[buffer_index] == '"') {
        buffer_index++;
        while (buffer[buffer_index] != '"') {
            next_word[i] = buffer[buffer_index];
            i++, buffer_index++;
        }
        buffer_index++;
    } else {
        while (buffer[buffer_index] != ' ' && buffer[buffer_index] != '\n' &&
               buffer[buffer_index] != '\0') {
            next_word[i] = buffer[buffer_index];
            i++, buffer_index++;
        }
    }

    next_word[i] = '\0';

    return (i == 0) ? NULL : next_word;
}

/*
 * Receives a list of lines and returns a new, alphabetically sorted list of
 * lines.
 */
line *sortLines(line lines_list[]) {

    int i, j, min_index;

    line *sorted_lines_list = malloc(nr_lines * sizeof(line));
    memcpy(sorted_lines_list, lines_list, nr_lines * sizeof(line));

    for (i = 0; i < nr_lines - 1; i++) {
        min_index = i;
        for (j = i + 1; j < nr_lines; j++) {
            if (strcmp(sorted_lines_list[j].line_name,
                       sorted_lines_list[min_index].line_name) < 0) {
                min_index = j;
            }
        }
        if (min_index != i) {
            line temp = sorted_lines_list[i];
            sorted_lines_list[i] = sorted_lines_list[min_index];
            sorted_lines_list[min_index] = temp;
        }
    }

    return sorted_lines_list;
}

/*
 * Checks if the input is an existing line in the system. Returns the
 * corresponding line index in the global line list if it is, -1 otherwise.
 */
int isLine(char l[]) {
    int i;

    for (i = 0; i < nr_lines; i++) {
        if (strcmp(l, line_list[i].line_name) == 0) {
            return i;
        }
    }

    return -1;
}

/*
 * List all the lines in the system.
 */
void listLines() {
    int i;

    for (i = 0; i < nr_lines; i++) {
        printf("%s ", line_list[i].line_name);

        if (line_list[i].nr_line_stops > 0) {
            printf(
                "%s %s ", line_list[i].course[0].stop_name,
                line_list[i].course[line_list[i].nr_line_stops - 1].stop_name);
        }

        printf("%d %.2f %.2f\n", line_list[i].nr_line_stops,
               line_list[i].total_cost, line_list[i].total_duration);
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
 * Prints all the stops in the line corresponnding to the index i in the global
 * line list.
 */
void listLineStops(int i, char buffer[]) {
    int j, reverse = 0;
    line current_line;
    char *flag;

    current_line = line_list[i];

    if (current_line.nr_line_stops == 0) {
        return;
    }

    /*Check if the command is reversed*/
    flag = readNextWord(buffer);
    if (flag != NULL) {
        if (isReverse(flag) == 0) {
            reverse = 1;
        } else {
            printf("incorrect sort option.\n");
            return;
        }
    }

    /*Print the stops*/
    if (reverse == 0) {
        for (j = 0; j < current_line.nr_line_stops - 1; j++) {
            printf("%s, ", current_line.course[j].stop_name);
        }
    } else {
        for (j = current_line.nr_line_stops - 1; j > 0; j--) {
            printf("%s, ", current_line.course[j].stop_name);
        }
    }
    printf("%s\n", current_line.course[j].stop_name);
}

/*
 * Creates a new line with the input name and adds it to the global line list.
 */
void createLine(char *line_name) {
    line new_line;

    strcpy(new_line.line_name, line_name);
    new_line.nr_line_stops = 0;
    new_line.total_cost = 0;
    new_line.total_duration = 0;
    new_line.is_cycle = 0;
    new_line.course = (stop *)malloc(2 * sizeof(stop));

    line_list[nr_lines] = new_line;
    nr_lines++;
}

/*
 * Main function that manages all the line commands.
 */
void lineCommand(char buffer[]) {

    char *l;
    int i;

    l = readNextWord(buffer);
    if (l == NULL) {
        listLines();
    } else {
        if ((i = isLine(l)) != -1) {
            listLineStops(i, buffer);
        } else {
            createLine(l);
        }
    }
}

/*
 * Prints the coordinates of the stop in the index i in the global stop list.
 */
void printStopCoords(int i) {
    printf("%16.12f %16.12f\n", stop_list[i].lat, stop_list[i].lon);
}

/*
 * Checks if the input is an existing stop in the system. Returns the
 * corresponding stop index in the global stop list if it is, -1 otherwise.
 */
int isStop(char s[]) {
    int i;

    for (i = 0; i < nr_stops; i++) {
        if (strcmp(s, stop_list[i].stop_name) == 0) {
            return i;
        }
    }

    return -1;
}

/*
 * Counts the number of lines that pass through the stop with the input name.
 */
int nrStopLines(char s[]) {

    int i, j, count = 0;

    for (i = 0; i < nr_lines; i++) {
        for (j = 0; j < line_list[i].nr_line_stops; j++) {
            if (strcmp(s, line_list[i].course[j].stop_name) == 0) {
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
void listStops() {

    int i;

    for (i = 0; i < nr_stops; i++) {
        printf("%s: %16.12f %16.12f %d\n", stop_list[i].stop_name,
               stop_list[i].lat, stop_list[i].lon,
               nrStopLines(stop_list[i].stop_name));
    }
}

/*
 * Creates a new stop with the input name, latitude and longitude and adds it to
 * the global stop list.
 */
void createStop(char *s, char *lat, char *lon) {
    stop new_stop;

    strcpy(new_stop.stop_name, s);
    new_stop.lat = atof(lat);
    new_stop.lon = atof(lon);

    stop_list[nr_stops] = new_stop;
    nr_stops++;
}

/*
 * Main function that manages all the stop commands, i.e. creating stops,
 * listing all stops and printing specific stop coordinates.
 */
void stopCommand(char buffer[]) {

    char *s, *lat, *lon;
    int i;

    s = readNextWord(buffer);
    if (s == NULL) {
        listStops();
    } else {
        if ((lat = readNextWord(buffer)) != NULL) {
            if (isStop(s) == -1) {
                lon = readNextWord(buffer);
                createStop(s, lat, lon);
            } else {
                printf("%s: stop already exists.\n", s);
                return;
            }
        } else {
            if ((i = isStop(s)) != -1) {
                printStopCoords(i);
            } else {
                printf("%s: no such stop.\n", s);
                return;
            }
        }
    }
}

/*
 * Checks whether the link attempted to create is valid for the given line.
 * Returns information represented by integers on the link type back to
 * isValidLink.
 */
int isLinkLineCompatible(int line_index, int origin_index,
                         int destination_index) {

    if (line_list[line_index].is_cycle == 1) {
        printf("link cannot be associated with bus line.\n");
        return -1;
    }

    if (strcmp(line_list[line_index]
                   .course[line_list[line_index].nr_line_stops - 1]
                   .stop_name,
               stop_list[origin_index].stop_name) == 0) {
        return 1;
    }

    if (strcmp(line_list[line_index].course[0].stop_name,

               stop_list[destination_index].stop_name) == 0) {
        return 0;
    }

    printf("link cannot be associated with bus line.\n");
    return -1;
}

/*
 * Checks if the link attempted to create has valid arguments. Returns -1 if
 * it's invalid, 0 if it's an origin link, 1 if it's a destination link and 2 if
 * these are the first stops to be added to the line course.
 */
int isValidLink(char *line_pre, char *origin_pre, char *destination_pre,
                double cost_pre, double duration_pre) {

    int line_index, origin_index, destination_index;

    line_index = isLine(line_pre);
    origin_index = isStop(origin_pre);
    destination_index = isStop(destination_pre);

    if (line_index == -1) {
        printf("%s: no such line.\n", line_pre);
        return -1;
    }

    if (origin_index == -1) {
        printf("%s: no such stop.\n", origin_pre);
        return -1;
    }

    if (destination_index == -1) {
        printf("%s: no such stop.\n", destination_pre);
        return -1;
    }

    if ((cost_pre < 0) || (duration_pre < 0)) {
        printf("negative cost or duration.\n");
        return -1;
    }

    if (line_list[line_index].nr_line_stops == 0) {
        return 2;
    }

    return isLinkLineCompatible(line_index, origin_index, destination_index);
}

/*
 * Function used to create the first 2 stops of the line of index i in the
 * global line list.
 */
void createFirstStops(int line_index, int origin_index, int destination_index,
                      double cost_pre, double duration_pre) {

    line_list[line_index].course[0] = stop_list[origin_index];
    line_list[line_index].course[1] = stop_list[destination_index];
    line_list[line_index].nr_line_stops = 2;
    line_list[line_index].total_cost = cost_pre;
    line_list[line_index].total_duration = duration_pre;
}

/*
 * Inserts a new stop either at the begining or at the end of the line of index
 * i in the global line list, depending on the link type.
 */
void createLink(int line_index, int origin_index, int destination_index,
                double cost_pre, double duration_pre, int link_info) {
    int i;

    switch (link_info) {
    /* Origin link. Stop is inserted in the begining of the line course. */
    case 0: {
        line_list[line_index].course = (stop *)realloc(
            line_list[line_index].course,
            (line_list[line_index].nr_line_stops + 1) * sizeof(stop));

        for (i = line_list[line_index].nr_line_stops - 1; i >= 0; i--) {
            line_list[line_index].course[i + 1] =
                line_list[line_index].course[i];
        }
        line_list[line_index].course[0] = stop_list[origin_index];
    } break;
    /* Destination link. Stop is inserted at the end of the line course. */
    case 1: {
        line_list[line_index].course = (stop *)realloc(
            line_list[line_index].course,
            (line_list[line_index].nr_line_stops + 1) * sizeof(stop));
        line_list[line_index].course[line_list[line_index].nr_line_stops] =
            stop_list[destination_index];
    } break;

    /* First stops. The mentioned link actually represents the first stops to be
     * inserted in the line course. */
    case 2: {
        createFirstStops(line_index, origin_index, destination_index, cost_pre,
                         duration_pre);
    }
        return;
    }

    line_list[line_index].nr_line_stops++;
    line_list[line_index].total_cost += cost_pre;
    line_list[line_index].total_duration += duration_pre;

    /* Final step to determine if the line becomes a cycle after adding the
     * link. */
    if (line_list[line_index].course[0].stop_name ==
        line_list[line_index]
            .course[line_list[line_index].nr_line_stops - 1]
            .stop_name) {
        line_list[line_index].is_cycle = 1;
    }
}

/*
 * Main link function that manages all link commands, i. e. creating links by
 * adding stops to line courses.
 */
void linkCommand(char buffer[]) {

    int line_index, origin_index, destination_index;
    double cost_pre, duration_pre;

    /* -1 if not possible, 0 if origin link, 1 if destination or cycle link, 2
     * if first stops.*/
    int link_info;
    char *line_pre, *origin_pre, *destination_pre;

    line_pre = readNextWord(buffer);
    origin_pre = readNextWord(buffer);
    destination_pre = readNextWord(buffer);

    line_index = isLine(line_pre);
    origin_index = isStop(origin_pre);
    destination_index = isStop(destination_pre);
    cost_pre = atof(readNextWord(buffer));
    duration_pre = atof(readNextWord(buffer));

    if ((link_info = isValidLink(line_pre, origin_pre, destination_pre,
                                 cost_pre, duration_pre)) != -1) {
        createLink(line_index, origin_index, destination_index, cost_pre,
                   duration_pre, link_info);
    }
    return;
}

/*
 * Intersection Command. Lists all the line intersections in the system.
 */
void intsecCommand() {

    int i, j, k;
    char *stop_pre;
    line *sorted_lines_list = sortLines(line_list);

    for (i = 0; i < nr_stops; i++) {
        stop_pre = stop_list[i].stop_name;
        if (nrStopLines(stop_pre) > 1) {
            printf("%s %d:", stop_pre, nrStopLines(stop_pre));
            for (j = 0; j < nr_lines; j++) {
                for (k = 0; k < sorted_lines_list[j].nr_line_stops; k++) {
                    if (strcmp(stop_pre,
                               sorted_lines_list[j].course[k].stop_name) == 0) {
                        printf(" %s", sorted_lines_list[j].line_name);
                        break;
                    }
                }
            }
            printf("\n");
        }
    }
}

/*
 * Main function. Inserts the input into the buffer and calls all the other main
 * command functions.
 */
int main() {

    char buffer[BUFFER_SIZE], c;
    int count;

    while (1) {

        memset(buffer, 0, BUFFER_SIZE);
        buffer_index = 0;

        for (count = 0; (c = getchar()) != '\n'; count++) {
            buffer[count] = c;
        }

        switch (buffer[0]) {
        case 'q':
            return 0;
        case 'c':
            lineCommand(buffer + 2);
            break;
        case 'p':
            stopCommand(buffer + 2);
            break;
        case 'l':
            linkCommand(buffer + 2);
            break;
        case 'i':
            intsecCommand();
            break;
        }
    }
}