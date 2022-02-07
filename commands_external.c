#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include "command_prompt.h"
#include "command_handlers.h"
#include "shell_process.h"
#include "smallsh.h"
#include "commands_external.h"

#define QUARTER_SECOND 250000

/*
 * Redirects uninitialized input and output to "/dev/null"
 */
void backgroundRedirection(struct Command *c) {
    char *dest = "/dev/null";

    if (c->input_redir == NULL) {
        stdinRedirectHandler(dest, c);
    }

    if (c->output_redir == NULL) {
        stdoutRedirectHandler(dest, c);
    }
}

/*
 * Executes command as a background process
 */
int executeExternalCommandBackground(struct Command *c, struct ShellProcess *sh) {
    struct sigaction ignore_action = {{0}};
	pid_t spawn_pid;
    
    backgroundRedirection(c);

    spawn_pid = fork();
	switch(spawn_pid) {
        case -1:
            perror("fork() failed!\n");
            fflush(stdout);
            return EXIT_FAILURE;
            break;
        case 0:
            // Additionally ignore SIGTSTP
            ignore_action.sa_handler = SIG_IGN;
            sigaction(SIGTSTP, &ignore_action, NULL);

            printf("Background pid is %d\n", getpid());
            fflush(stdout);
            execvp(c->name, c->args);

            // Only occurs during an unsuccessful exec()
            perror(c->name);
            fflush(stdout);
            kill(getpid(), SIGKILL);
            return EXIT_FAILURE;
            break;
        default:
            usleep(QUARTER_SECOND);  // Allow child process to execute exec() before parent exits
            addBackgroundProcess(spawn_pid, sh);
            return EXIT_SUCCESS;
            break;
	}
}

/*
 * Executes command as a foreground process
 */
int executeExternalCommandForeground(struct Command *c, struct ShellProcess *sh) {
    struct sigaction SIGINT_action = {{0}}, ignore_action = {{0}};
	pid_t spawn_pid;
    
    spawn_pid = fork();

	switch(spawn_pid) {
        case -1:
            perror("fork() failed!\n");
            fflush(stdout);
            return EXIT_FAILURE;
            break;
        case 0:
            // Terminate after receiving SIGINT
            SIGINT_action.sa_handler = SIG_DFL;
            sigaction(SIGINT, &SIGINT_action, NULL);

            // Additionally ignore SIGTSTP
            ignore_action.sa_handler = SIG_IGN;
            sigaction(SIGTSTP, &ignore_action, NULL);
            
            execvp(c->name, c->args);

            // Only occurs during an unsuccessful exec()
            perror(c->name);
            fflush(stdout);
            kill(getpid(), SIGKILL);
            return EXIT_FAILURE;
            break;
        default:
            setForegroundPID(spawn_pid);
            raise(SIGUSR2);
            return EXIT_SUCCESS;
            break;
	}
}

/*
 * Checks if command is to be executed as a background process
 */
int runInBackgroundAttempt(struct Command *c) {
    return c->foreground == 1;
}