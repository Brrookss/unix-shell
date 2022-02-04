#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "command_prompt.h"

/*
 *
 */
int executeExternalCommand(struct Command *c) {
	int child_status;

	pid_t spawn_pid = fork();

	switch(spawn_pid) {
        case -1:
            perror("fork()\n");
            return EXIT_FAILURE;
            break;
        case 0:
            execvp(c->name, c->args);
            perror("execvp");
            return EXIT_FAILURE;
            break;
        default:
            spawn_pid = waitpid(spawn_pid, &child_status, 0);
            return EXIT_SUCCESS;
            break;
	}
}