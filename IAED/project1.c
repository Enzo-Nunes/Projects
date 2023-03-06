/* iaed-23 - ist1106336 - project1 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUS_NAME_SIZE 20
#define BUFFER_SIZE 8193

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

            case 'p':

            case 'l':

            case 'i':
            
        }
    }

    return 0;
}