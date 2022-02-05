#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "shell_process.h"

/*
 * Initializes a ShellProcess structure
 */
struct ShellProcess *initializeShellProcessStruct(void) {
    struct ShellProcess *sh;

    sh = (struct ShellProcess *)malloc(sizeof(struct ShellProcess));

    sh->head = NULL;
    sh->prev_term_signal = 0;
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
 * Sets the ShellProcess structure's most recent terminating signal value
 */
void setPrevTermSignal(int child_status, struct ShellProcess *sh) {
    if (WIFEXITED(child_status)) {
        sh->prev_term_signal = WEXITSTATUS(child_status);
    } else {
        sh->prev_term_signal = WTERMSIG(child_status);
    }
    fflush(stdout);
}