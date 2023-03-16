/* iaed-23 - ist1106336 - project1 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE BUFSIZ
#define STOP_NAME_SIZE 50
#define LINE_NAME_SIZE 20
#define MAX_STOPS 10000
#define MAX_LINES 200

typedef struct {
    char stop_name[STOP_NAME_SIZE];
    double lat;
    double lon;
} stop;

typedef struct {
    char line_name[LINE_NAME_SIZE];
    stop course[MAX_STOPS];
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

/* Function that reads the next word in the buffer and returns it. */
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

int isLine(char l[]) {
    int i;

    for (i = 0; i < nr_lines; i++) {
        if (strcmp(l, line_list[i].line_name) == 0) {
            return i;
        }
    }

    return -1;
}

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

void listLineStops(int i, char buffer[]) {
    int j, reverse = 0;
    line current_line;
    char *flag;

    current_line = line_list[i];

    /*Check if the command is reversed*/
    flag = readNextWord(buffer);
    if (flag != NULL) {
        if (strncmp(flag, "inverso", 3) == 0) {
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

void createLine(char *l) {
    line new_line;

    strcpy(new_line.line_name, l);
    new_line.nr_line_stops = 0;
    new_line.total_cost = 0;
    new_line.total_duration = 0;
    new_line.is_cycle = 0;

    line_list[nr_lines] = new_line;
    nr_lines++;
}

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

void printStopCoords(int i) {
    printf("%16.12f %16.12f\n", stop_list[i].lat, stop_list[i].lon);
}

int isStop(char s[]) {
    int i;

    for (i = 0; i < nr_stops; i++) {
        if (strcmp(s, stop_list[i].stop_name) == 0) {
            return i;
        }
    }

    return -1;
}

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

void listStops() {

    int i;

    for (i = 0; i < nr_stops; i++) {
        printf("%s: %16.12f %16.12f %d\n", stop_list[i].stop_name,
               stop_list[i].lat, stop_list[i].lon,
               nrStopLines(stop_list[i].stop_name));
    }
}

void createStop(char *s, char *lat, char *lon) {
    stop new_stop;

    strcpy(new_stop.stop_name, s);
    new_stop.lat = atof(lat);
    new_stop.lon = atof(lon);

    stop_list[nr_stops] = new_stop;
    nr_stops++;
}

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

void createFirstStops(int line_index, int origin_index, int destination_index,
                      double cost_pre, double duration_pre) {

    line_list[line_index].course[0] = stop_list[origin_index];
    line_list[line_index].course[1] = stop_list[destination_index];
    line_list[line_index].nr_line_stops = 2;
    line_list[line_index].total_cost = cost_pre;
    line_list[line_index].total_duration = duration_pre;
}

void createLink(int line_index, int origin_index, int destination_index,
                double cost_pre, double duration_pre, int link_info) {

    int i;
    switch (link_info) {
    case 0: {
        for (i = line_list[line_index].nr_line_stops - 1; i >= 0; i--) {
            line_list[line_index].course[i + 1] =
                line_list[line_index].course[i];
        }
        line_list[line_index].course[0] = stop_list[origin_index];
    } break;

    case 1: {
        line_list[line_index].course[line_list[line_index].nr_line_stops] =
            stop_list[destination_index];
    } break;

    case 2: {
        createFirstStops(line_index, origin_index, destination_index, cost_pre,
                         duration_pre);
    }
        return;
    }

    line_list[line_index].nr_line_stops++;
    line_list[line_index].total_cost += cost_pre;
    line_list[line_index].total_duration += duration_pre;

    if (line_list[line_index].course[0].stop_name ==
        line_list[line_index]
            .course[line_list[line_index].nr_line_stops - 1]
            .stop_name) {
        line_list[line_index].is_cycle = 1;
    }
}

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