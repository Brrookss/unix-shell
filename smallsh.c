#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "shell_process.h"
#include "command_prompt.h"
#include "command_handlers.h"
#include "commands_built_in.h"
#include "commands_external.h"
#include "memory.h"
#include "signal_handlers.h"

int main(void) {
    struct ShellProcess *sh;
    sh = initializeShellProcess();

    while (isRunning(sh)) {
        struct Command *c;
        char *input;
        int saved_stdin, saved_stdout;

        checkBackgroundProcesses(sh);

        input = getInput();
        c = parseInput(input);

        saved_stdin = saved_stdout = 0;

        if (stdinRedirectAttempt(c))
            saved_stdin = redirectStdin(c);
        if (stdoutRedirectAttempt(c))
            saved_stdout = redirectStdout(c);

        if (!successfulRedirects(saved_stdin, saved_stdout))
            setExitFailureMessage(sh);

        if (isCommand(c) && successfulRedirects(saved_stdin, saved_stdout)) {
            if (isBuiltInCommand(c)) {
                executeBuiltInCommand(c, sh);
            } else {
                runInForeground(c) ? executeExternalCommandForeground(c, sh) : executeExternalCommandBackground(c, sh);
            }
        }

        if (stdinRedirectAttempt(c))
            resetStdin(saved_stdin);
        if (stdoutRedirectAttempt(c))
            resetStdout(saved_stdout);

        deallocateCommand(c);
        deallocateInput(input);
    }
    deallocateShellProcess(sh);
    return EXIT_SUCCESS;
}