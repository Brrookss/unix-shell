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
#include "shell.h"

// To be accessible by signal handlers
int foreground_pid;
int foreground_only = 1;
char* status_message = NULL;

int main(void) {
    struct ShellProcess* sh;
    sh = initializeShellProcess();

    initializeSignalHandlers();

    while (isRunning(sh)) {
        struct Command* c;
        char* input;
        int saved_stdin;
        int saved_stdout;

        checkBackgroundProcesses(sh);

        input = getInput();
        c = parseInput(input);

        saved_stdin = STDIN_FILENO;
        saved_stdout = STDOUT_FILENO;

        if (stdinRedirectAttempt(c))
            saved_stdin = redirectStdin(c);
        if (stdoutRedirectAttempt(c))
            saved_stdout = redirectStdout(c);

        if (!successfulRedirects(saved_stdin, saved_stdout))
            setExitFailureMessage(sh);

        if (isCommand(c) && successfulRedirects(saved_stdin, saved_stdout)) {
            if (isBuiltInCommand(c)) {
                executeBuiltInCommand(c, sh);
            } else if (runInBackgroundAttempt(c) && backgroundProcessesAllowed()) {
                executeExternalCommandBackground(c, sh);
            } else {
                executeExternalCommandForeground(c, sh);

                if (hasDifferentStatusMessage(status_message, sh)) {
                    updatePrevStatusMessage(status_message, sh);
                }
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

/**
 * Enables and disables foreground-only mode as a result of receiving the
 * SIGTSTP signal.
 * 
 * 0 and 1 refer to enabled and disabled, respectively.
 */
void alternateForegroundOnly(void) {
    char* prompt = ": ";

    foreground_only = foreground_only == 0 ? 1 : 0;

    printf("\n");
    fflush(stdout);

    if (foreground_only == 0) {
        printf("Entering foreground-only mode (& is now ignored)\n");
    } else {
        printf("Exiting foreground-only mode\n");
    }
    fflush(stdout);

    printf("%s", prompt);
    fflush(stdout);
}

/**
 * Checks if executing processes in the background is allowed based on the
 * value of the foreground_only global integer.
 * 
 * Its value is dictated by the SIGTSTP signal.
 */
int backgroundProcessesAllowed(void) {
    return foreground_only == 1;
}

/**
 * Gets the PID of the most recent foreground process.
 */
int getForegroundPID(void) {
    return foreground_pid;
}

/**
 * Sets the PID of the most recent foreground process.
 */
void setForegroundPID(int pid) {
    foreground_pid = pid;
}

/**
 * Sets the terminating signal value of the most recent foreground process.
 */
void setStatusMessage(char* s) {
    status_message = s;
}