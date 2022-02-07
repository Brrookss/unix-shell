#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include "signal_handlers.h"
#include "shell_process.h"
#include "smallsh.h"

/*
 * Sets the global foreground integer which represents whether
 * background proccesses are allowed to be invoked
 */
void handleSIGTSTP(int signo) {
    write(STDOUT_FILENO, "\n", 1);
    alternateForegroundOnly();
}

/*
 * Waits for a child process to terminate, gets its terminating status message,
 * and sets the global status message. If the process is terminated by SIGINT,
 * a message is displayed
 */
void handleSIGUSR2(int signo) {
    int child_status, status;
    pid_t spawn_pid;
	char *s;

	spawn_pid = waitpid(-1, &child_status, 0);
    
    s = (char *)calloc(STATUS_MESSAGE_CHARS, sizeof(char));
    
    if (WIFEXITED(child_status)) {
        sprintf(s, "Exit value %d", WEXITSTATUS(child_status));
		status = WEXITSTATUS(child_status);
    } else {
        sprintf(s, "Terminated by signal %d", WTERMSIG(child_status));
		status = WTERMSIG(child_status);
    }

	if (terminatedBySIGINT(status)) {
		write(STDOUT_FILENO, s, strlen(s));
		write(STDOUT_FILENO, "\n", 1);
	}
	
	setStatusMessage(s);
}

/*
 * Initializes the SIGINT signal handler
 */
void initializeSIGINT(void) {
	struct sigaction SIGINT_action = {0};

	SIGINT_action.sa_handler = SIG_IGN;
	sigfillset(&SIGINT_action.sa_mask);

	sigaction(SIGINT, &SIGINT_action, NULL);
}

/*
 * Initializes the SIGTSTP signal handler
 */
void initializeSIGTSTP(void) {
	struct sigaction SIGTSTP_action = {0};

	SIGTSTP_action.sa_handler = handleSIGTSTP;
	sigfillset(&SIGTSTP_action.sa_mask);

	sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}

/*
 * Initializes all signal handlers associated with the shell
 */
void initializeSignalHandlers(void) {
	initializeSIGINT();
    initializeSIGTSTP();
	initializeSIGUSR2();
}

/*
 * Initializes the SIGUSR2 signal handler
 */
void initializeSIGUSR2(void) {
	struct sigaction SIGUSR2_action = {0};

	SIGUSR2_action.sa_handler = handleSIGUSR2;
	sigfillset(&SIGUSR2_action.sa_mask);
	
	sigaction(SIGUSR2, &SIGUSR2_action, NULL);
}

/*
 * Checks if status matches the SIGINT terminating status
 */
int terminatedBySIGINT(int status) {
	return status == 2;
}