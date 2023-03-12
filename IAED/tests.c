#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

char* readNextWord(char buffer[]) {
    static int index = 0;
    int i = 0;
    char next_word[BUFSIZ];

    while (buffer[index] == ' ' || buffer[index] == '\n') {
        index++;
    }

    for (i = 0; buffer[index] != ' ' && buffer[index] != '\n' && buffer[index] != '\0'; i++, index++) {
        next_word[i] = buffer[index];
    }

    return next_word;
}

int main() {

    char buffer[BUFSIZ], c;
    int i, count = 0;

    memset(buffer, 0, BUFSIZ);

    while (c = getchar() != '\n') {
        buffer[count] = c;
        count++;
    }
    buffer[count] = '\0';

    return 0;
}