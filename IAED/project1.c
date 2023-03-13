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


typedef struct {
    char stop_name[STOP_NAME_SIZE];
    float lat;
    float lon;
} stop;

typedef struct {
    char line_name[LINE_NAME_SIZE];
    stop course[MAX_STOPS];
    int nr_line_stops;
    float total_cost, total_duration;
} line;


line line_list[MAX_LINES];
stop stop_list[MAX_STOPS];

int nr_lines  = 0;
int nr_stops  = 0;
int nr_intsec = 0;


char* readNextWord(char buffer[]) {
    static int index = 0;
    int i = 0;
    char* next_word = malloc(BUFSIZ);

    while (buffer[index] == ' ' || buffer[index] == '\n') {
        index++;
    }

    while (
        buffer[index] != ' ' &&
        buffer[index] != '\n' &&
        buffer[index] != '\0'
    ) {
        next_word[i] = buffer[index];
        i++, index++;
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
            printf("%s %s ",
                line_list[i].course[0].stop_name,
                line_list[i].course[line_list[i].nr_line_stops - 1].stop_name);
        }

        printf( "%d %.2f %.2f\n",
                line_list[i].nr_line_stops,
                line_list[i].total_cost,
                line_list[i].total_duration
        );
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

    if (readNextWord(buffer) == NULL) {
        listLines();
    } else {
        l = readNextWord(buffer);
        if ((i = isLine (l)) != -1) {
            listLineStops(i, buffer);
        } else {
            createLine(l);
        }
    }
}


int main() {

    char buffer[BUFFER_SIZE], c;
    int count;

    while (1) {

        memset(buffer, 0, BUFFER_SIZE);

        for (count = 0; (c = getchar()) != '\n'; count++) {
            buffer[count] = c;
        }

        switch (buffer[0]) {
            case 'q':
                return 0;
            case 'c':
                lineCommand(buffer);
                break;
        }
    }
}