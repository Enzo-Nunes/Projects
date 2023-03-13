/* iaed-23 - ist1106336 - project1 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE BUFSIZ
#define STOP_NAME_SIZE 50
#define LINE_NAME_SIZE 20
#define MAX_STOPS 10000
#define MAX_LINES 200


stop stop_list[MAX_STOPS];
int nr_stops = 0;

line line_list[MAX_LINES];
int nr_lines = 0;


typedef struct {
    char line_name[LINE_NAME_SIZE];
    /*char origin[STOP_NAME_SIZE];
    char destination[STOP_NAME_SIZE];*/
    char course[MAX_STOPS];
    int nr_line_stops;
    float total_cost, total_duration;
} line;

typedef struct {
    char stop_name[STOP_NAME_SIZE];
    float lat;
    float lon;
} stop;


char* readNextWord(char buffer[]) {
    static int index = 0;
    int i = 0;
    char* next_word = malloc(BUFSIZ);

    while (buffer[index] == ' ' || buffer[index] == '\n') {
        index++;
    }

    for (;
        buffer[index] != ' ' &&
        buffer[index] != '\n' &&
        buffer[index] != '\0';
        i++, index++
        ) {
        next_word[i] = buffer[index];
    }

    next_word[i] = '\0';

    return (i == 0) ? NULL : next_word;
}


void lineCommand(char buffer[]) {

    char *l;

    if (readNextWord(buffer) == NULL) {
        listLines();
    } else {
        l = readNextWord(buffer);
        if (isLine (l) == 1) {
            listLineStops(l);
        } else {
            createLine(l, buffer);
        }
    }
}

int isLine(char l[]) {
    int i;

    for (i = 0; i < nr_lines; i++) {
        if (strcmp(l, line_list[i].line_name) == 0) {
            return 1;
        }
    }

    return 0;
}

void listLines() {
    int i;

    for (i = 0; i < nr_lines; i++) {
        printf(
            "%s %s %s %d %.2f %.2f\n",
                line_list[i].line_name,
                line_list[i].course[0],
                line_list[i].course[line_list[i].nr_line_stops - 1],
                line_list[i].nr_line_stops,
                line_list[i].total_cost,
                line_list[i].total_duration
        );
    }
}

void listLineStops(char l[]) {
    /*Cause I'm gay*/
}

void createLine(char l, char buffer[]) {
    line l;
}



int main() {

    char buffer[BUFFER_SIZE], c;
    int count = 0;

    while (1) {

        memset(buffer, 0, BUFFER_SIZE);

        while ((c = getchar()) != '\n') {
            buffer[count] = c;
            count++;
        }
        buffer[count] = '\0';

        switch (buffer[0]) {
            case 'q':
                return 0;
            case 'c':
                lineCommand(buffer);
                break;
            case 'p':
                stopCommand(buffer);
                break;
            case 'l':
                linkCommand(buffer);
                break;
            case 'i':
                intsecCommand(buffer);
                break;
        }
    }

    return 0;
}