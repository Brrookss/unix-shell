#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "command_prompt.h"
#include "command_handlers.h"
#include "commands_built_in.h"
#include "commands_external.h"
#include "memory.h"

int main(void) {
    while (1) {
        struct Command *c;
        char *input;

        input = getInput();
        c = parseInput(input);

        if (isCommand(c) && isBuiltInCommand(c)) {
            executeBuiltInCommand(c);
        } else if (isCommand(c)) {
            executeExternalCommand(c);
        }

        deallocateInput(input);
        deallocateCommandStruct(c);
    }
    return EXIT_SUCCESS;
}