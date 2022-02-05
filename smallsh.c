#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "shell_process.h"
#include "command_prompt.h"
#include "command_handlers.h"
#include "commands_built_in.h"
#include "signal_handlers.h"
#include "commands_external.h"
#include "memory.h"

int main(void) {
    struct ShellProcess *sh;
    sh = initializeShellProcessStruct();

    while (sh->exiting != 0) {
        struct Command *c;
        char *input;

        checkBackgroundProcesses(sh);

        input = getInput();
        c = parseInput(input);

        if (isCommand(c) && isBuiltInCommand(c)) {
            executeBuiltInCommand(c, sh);
        } else if (isCommand(c)) {
            c->foreground == 0 ? executeExternalCommandForeground(c, sh) : executeExternalCommandBackground(c, sh);
        }
        deallocateInput(input);
        deallocateCommandStruct(c);
    }
    deallocateShellProcessStruct(sh);
    return EXIT_SUCCESS;
}