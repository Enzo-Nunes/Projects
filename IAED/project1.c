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
    int nr_line_stops;
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
int nr_intsec = 0;

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
        for (j = 0; j < current_line.nr_line_stops; j++) {
            printf("%s\n", current_line.course[j].stop_name);
        }
    } else {
        for (j = current_line.nr_line_stops - 1; j >= 0; j--) {
            printf("%s\n", current_line.course[j].stop_name);
        }
    }
}

void createLine(char *l) {
    line new_line;

    strcpy(new_line.line_name, l);
    new_line.nr_line_stops = 0;
    new_line.total_cost = 0;
    new_line.total_duration = 0;

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

void linkCommand(char buffer[]) {

    char *l, *s;

    l = readNextWord(buffer);
    if (isLine(l) == -1) {
        printf("%s: no such line.\n", l);
        return;
    } else {
        s = readNextWord(buffer);
        if (isStop(s) == -1) {
            printf("%s: no such stop.\n", s);
            return;
        } else {
            if () {
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
        }
    }
}