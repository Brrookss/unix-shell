#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "command_prompt.h"

int main(void) {
    while (1) {
        struct Command *c;
        char *input;

        input = getInput();
        c = parseInput(input);

        free(c);
        c = NULL;
    }

    return 0;
}