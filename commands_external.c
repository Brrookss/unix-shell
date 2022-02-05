#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "command_prompt.h"
#include "command_handlers.h"
#include "shell_process.h"

/*
 * Redirects uninitialized input and output to "/dev/null"
 */
void backgroundRedirection(struct Command *c) {
    char *dest = "/dev/null";

    if (c->iredir == NULL) {
        inputRedirectHandler(dest, c);
    }

    if (c->oredir == NULL) {
        outputRedirectHandler(dest, c);
    }
}

/*
 * Executes command as a background process
 */
int executeExternalCommandBackground(struct Command *c, struct ShellProcess *sh) {
	pid_t spawn_pid;
    
    backgroundRedirection(c);

    spawn_pid = fork();

	switch(spawn_pid) {
        case -1:
            perror("fork() failed!\n");
            return EXIT_FAILURE;
            break;
        case 0:
            printf("Background pid is %d\n", getpid());
            fflush(stdout);
            execvp(c->name, c->args);
            perror(c->name);
            return EXIT_FAILURE;
            break;
        default:
            addBackgroundProcess(spawn_pid, sh);
            return EXIT_SUCCESS;
            break;
	}
}

/*
 * Executes command as a foreground process
 */
int executeExternalCommandForeground(struct Command *c, struct ShellProcess *sh) {
	int child_status;
	pid_t spawn_pid;
    
    spawn_pid = fork();

	switch(spawn_pid) {
        case -1:
            perror("fork() failed!\n");
            return EXIT_FAILURE;
            break;
        case 0:
            execvp(c->name, c->args);
            perror(c->name);
            return EXIT_FAILURE;
            break;
        default:
            spawn_pid = waitpid(spawn_pid, &child_status, 0);
            setPrevStatusMessage(child_status, sh);
            return EXIT_SUCCESS;
            break;
	}
}

/*
 * Checks if command is to be executed as a foreground process
 */
int runInForeground(struct Command *c) {
    return c->foreground == 0;
}