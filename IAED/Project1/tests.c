#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

char* readNextWord(char buffer[]) {
    static int index = 0;
    int i = 0;
    char* next_word = malloc(BUFSIZ);

    while (buffer[index] == ' ' || buffer[index] == '\n') {
        index++;
    }

    for (;buffer[index] != ' ' && buffer[index] != '\n' && buffer[index] != '\0'; i++, index++) {
        next_word[i] = buffer[index];
    }

    next_word[i] = '\0';

    return (i == 0) ? NULL : next_word;
}

int main() {

    char buffer[BUFSIZ], c, *word;
    int count = 0;

    memset(buffer, 0, BUFSIZ);

    while ((c = getchar()) != '\n') {
        buffer[count] = c;
        count++;
    }
    buffer[count] = '\0';

    while ((word = readNextWord(buffer)) != NULL) {
        printf("%s\n", word);
    }

    return 0;
}