#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "memory.h"

/*
 * Deallocates user input character array
 */
void deallocateInput(char *s) {
    free(s);
    s = NULL;
}

/*
 * Deallocates Command structure including all dynamically created arguments
 */
void deallocateCommand(struct Command *c) {
    int i;

    if (c->name) {
        free(c->name);
        c->name = NULL;
    }

    for (i = 0; i < MAX_ARGS + 1; i++) {
        free(c->args[i]);
        c->args[i] = NULL;
    }

    if (c->iredir) {
        free(c->iredir);
        c->iredir = NULL;
    }

    if (c->oredir) {
        free(c->oredir);
        c->oredir = NULL;
    }
    free(c);
    c = NULL;
}

/*
 * Deallocates ShellProcess structure and terminates all
 * currently running background processes
 */
void deallocateShellProcess(struct ShellProcess *sh) {
    struct BackgroundProcess *cur, *next;

    cur = sh->head;

    while (cur) {
        next = cur->next;
        kill(cur->pid, SIGTERM);
        free(cur);
        cur = next;
    }

    free(sh->prev_status_message);
    sh->prev_status_message = NULL;

    free(sh);
    sh = NULL;
}