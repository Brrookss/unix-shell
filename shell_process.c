#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell_process.h"

/**
 * Adds a BackgroundProcess struct to the ShellProcess struct.
 */
void addBackgroundProcess(int pid, struct ShellProcess* sh) {
    struct BackgroundProcess* cur;
    struct BackgroundProcess* prev;
    struct BackgroundProcess* new;

    new = (struct BackgroundProcess*)malloc(sizeof(struct BackgroundProcess));
    new->pid = pid;

    cur = sh->head;
    prev = NULL;

    while (cur) {
        prev = cur;
        cur = cur->next;
    }
    
    if (cur == sh->head) {
        sh->head = new;
    } else {
        prev->next = new;
    }
    new->next = NULL;
}

/**
 * Checks the status of any recently ended background processes.
 */
void checkBackgroundProcesses(struct ShellProcess* sh) {
    int child_status, status;
    pid_t spawn_pid;

    spawn_pid = waitpid(-1, &child_status, WNOHANG);
    
    if (spawn_pid > 0) {
        status = deleteBackgroundProcess(spawn_pid, sh);

        if (foundBackgroundProcess(status)) {
            if (WIFEXITED(child_status)) {
                printf("Background pid %d is done: Exit value %d\n", spawn_pid, WEXITSTATUS(child_status));
            } else {
                printf("Background pid %d is done: Terminated by signal %d\n", spawn_pid, WTERMSIG(child_status));
            }
            fflush(stdout);
        }
    }
}

/**
 * Deletes a BackgroundProcess struct from the ShellProcess struct.
 */
int deleteBackgroundProcess(int pid, struct ShellProcess* sh) {
    struct BackgroundProcess* cur;
    struct BackgroundProcess* prev;
    int found;

    cur = sh->head;
    prev = NULL;

    found = -1;

    while (cur && cur->pid != pid) {
        prev = cur;
        cur = cur->next;
    }

    if (cur) {
        if (cur == sh->head) {
            sh->head = cur->next;
        } else {
            prev->next = cur->next;
        }
        kill(pid, SIGINT);
        free(cur);
        cur = NULL;
        found = 0;
    }
    return found;
}

/**
 * Displays the most recent terminating signal value.
 */
void displayPrevStatusMessage(struct ShellProcess* sh) {
    printf("%s\n", sh->prev_status_message);
    fflush(stdout);
}

/**
 * Initializes a ShellProcess struct.
 */
struct ShellProcess* initializeShellProcess(void) {
    struct ShellProcess* sh;
    char* s;

    sh = (struct ShellProcess*)malloc(sizeof(struct ShellProcess));

    s = (char*)calloc(STATUS_MESSAGE_CHARS, sizeof(char));
    strcpy(s, "Exit status 0");

    sh->head = NULL;
    sh->prev_status_message = s;
    sh->exiting = 1;

    return sh;
}

/**
 * Checks if a BackgroundProcess struct was found in the ShellProcess struct.
 */
int foundBackgroundProcess(int found) {
    return found == 0;
}

/**
 * Checks ShellProcess exit status.
 */
int isRunning(struct ShellProcess* sh) {
    return sh->exiting != 0;
}

/**
 * Sets the ShellProcess struct's most recent status to a failure message.
 */
void setExitFailureMessage(struct ShellProcess* sh) {
    char* s;

    if (sh->prev_status_message)
        free(sh->prev_status_message);

    s = (char*)calloc(STATUS_MESSAGE_CHARS, sizeof(char));
    strcpy(s, "Exit value 1");

    sh->prev_status_message = s;
}

/**
 * Checks if the ShellProcess struct's most recent status differs.
 */
int hasDifferentStatusMessage(char* s, struct ShellProcess* sh) {
    return strcmp(s, sh->prev_status_message) != 0;
}

/**
 * Sets the ShellProcess structure's most recent terminating signal value.
 * 
 * The value returned is an integer representing the exit or terminating
 * status as determined by WIFEXITED().
 */
int setPrevStatusMessage(int child_status, struct ShellProcess* sh) {
    int status;
    char* s;

    if (sh->prev_status_message)
        free(sh->prev_status_message);
    
    s = (char*)calloc(STATUS_MESSAGE_CHARS, sizeof(char));
    
    if (WIFEXITED(child_status)) {
        sprintf(s, "Exit value %d", WEXITSTATUS(child_status));
        status = WEXITSTATUS(child_status);
    } else {
        sprintf(s, "Terminated by signal %d", WTERMSIG(child_status));
        status = WTERMSIG(child_status);
    }
    sh->prev_status_message = s;
    return status;
}

/**
 * Sets the ShellProcess struct's most recent terminating signal to a message.
 * 
 * This differs from setPrevStatusMessage() in that this version does not
 * determine the message to be stored.
 */
void updatePrevStatusMessage(char* s, struct ShellProcess* sh) {
    if (sh->prev_status_message)
        free(sh->prev_status_message);
    sh->prev_status_message = s;
}