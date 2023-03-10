/* iaed-23 - ist1106336 - project1 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE BUFSIZ
#define BUS_NAME_SIZE 20
#define STOP_NAME_SIZE 50
#define LINE_NAME_SIZE 20

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

int main() {

    char buffer[BUFFER_SIZE], c;
    int count = 0;

    while (1) {
        while (c = getchar() != '\n') {
            buffer[count] = c;
            count++;
        }
        buffer[count] = '\0';

        switch (buffer[0]) {
            case 'q':
                return 0;
            case 'c':
                criaCarreira(buffer);
            case 'p':

            case 'l':

            case 'i':
            
        }
    }

    return 0;
}