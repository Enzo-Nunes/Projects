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
    char stop_name[STOP_NAME_SIZE];
    float lat;
    float lon;
} stop;

typedef struct {
    char line_name[LINE_NAME_SIZE];
    stop origin;
    stop destination;


} line;

typedef struct {
    line l;
    stop origin;
    stop destination;
    float cost;
    float duration;
} link;


char readNextWord(char buffer[]) {

    int i;
    char next_word[LINE_NAME_SIZE];

    for (i = 2; i < (int) strlen(buffer); i++) {
        if (buffer[i] != ' ' && buffer[i] != '\n') {
            next_word[i - 2] = buffer[i];
        }
    }
    next_word[i - 1] = '\0';

    return next_word;
}



void listLines() {
    int i;

    for (i = 0; i < nr_lines; i++) {
        printf("%s", line_list[i].line_name);
    }
}

void createLine(char buffer[]) {
    line l;
}

int main() {

    char buffer[BUFFER_SIZE], c, l;
    int i, count = 0;

    while (1) {

        memset(buffer, 0, BUFFER_SIZE);

        while (c = getchar() != '\n') {
            buffer[count] = c;
            count++;
        }
        buffer[count] = '\0';

        switch (buffer[0]) {
            case 'q':
                return 0;
            case 'c':
                if (buffer[2] == '\0') {
                    listLines();
                } else {

                    l = readNextWord(buffer);
                    if (isLine (l)) {
                        listLineStops(l, buffer);
                    } else {
                        createLine(l);
                    }

                    for (i = 0; i < nr_lines; i++) {
                        if (strcmp(l, line_list[i].line_name) == 0) {
                            listLineStops(l, buffer);
                            break;
                        }
                    }

                }
                break;
            case 'p':

            case 'l':

            case 'i':
            
        }
    }

    return 0;
}