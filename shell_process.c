#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "shell_process.h"

#define STATUS_MESSAGE_CHARS 100

/*
 * Initializes a ShellProcess structure
 */
struct ShellProcess *initializeShellProcess(void) {
    struct ShellProcess *sh;
    char *s;

    sh = (struct ShellProcess *)malloc(sizeof(struct ShellProcess));

    s = (char *)calloc(STATUS_MESSAGE_CHARS, sizeof(char));
    strcpy(s, "Exit status 0");

    sh->head = NULL;
    sh->prev_status_message = s;
    sh->exiting = 1;

    return sh;
}

/*
 * Adds a BackgroundProcess structure to the linked list stored
 * in the ShellProcess structure
 */
void addBackgroundProcess(int pid, struct ShellProcess *sh) {
    struct BackgroundProcess *cur, *prev, *new;

    new = (struct BackgroundProcess *)malloc(sizeof(struct BackgroundProcess));
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

/*
 * Checks the status of any recently ended background processes
 */
void checkBackgroundProcesses(struct ShellProcess *sh) {
    int child_status;
    pid_t spawn_pid;

    while ((spawn_pid = waitpid(-1, &child_status, WNOHANG)) > 0) {
        deleteBackgroundProcess(spawn_pid, sh);
        if (WIFEXITED(child_status)) {
            printf("Background pid %d is done: Exit value %d\n", spawn_pid, WEXITSTATUS(child_status));
        } else {
            printf("Background pid %d is done: Terminated by signal %d\n", spawn_pid, WTERMSIG(child_status));
        }
        fflush(stdout);
    }
}

/*
 * Deletes a BackgroundProcess structure from the linked list in ShellProcess
 */
void deleteBackgroundProcess(int pid, struct ShellProcess *sh) {
    struct BackgroundProcess *cur, *prev;

    cur = sh->head;
    prev = NULL;

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
    }
}

/*
 * Checks ShellProcess exit status
 */
int isRunning(struct ShellProcess *sh) {
    return sh->exiting != 0;
}

/*
 * Sets the ShellProcess structure's most recent terminating signal value
 */
void setPrevStatusMessage(int child_status, struct ShellProcess *sh) {
    char *s;

    if (sh->prev_status_message)
        free(sh->prev_status_message);
    
    s = (char *)calloc(STATUS_MESSAGE_CHARS, sizeof(char));
    
    if (WIFEXITED(child_status)) {
        sprintf(s, "Exit value %d", WEXITSTATUS(child_status));
    } else {
        sprintf(s, "Terminated by signal %d", WTERMSIG(child_status));
    }
    sh->prev_status_message = s;
}