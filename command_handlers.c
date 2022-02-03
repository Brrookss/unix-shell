#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_prompt.h"
#include "command_handlers.h"

/*
 * Adds to the Command struct's argument character pointer array
 * with the index being the first NULL element.
 */
void argumentHandler(char *input, struct Command *c) {
    char *s;
    int i;

    s = (char *)calloc(strlen(input) + 1, sizeof(char));
    strcpy(s, input);

    i = 0;
    while (c->args[i]) {
        i += sizeof(char *);
    }
    c->args[i] = s;
}

/*
 * Sets the Command struct's name character pointer.
 * Only the first instantiation is used.
 */
void commandHandler(char *input, struct Command *c) {
    if (c->name == NULL) {
        char *s;

        s = (char *)calloc(sizeof(input + 1), sizeof(char));
        strcpy(s, input);

        c->name = s;
    }
}

/*
 * Sets the Command struct's input redirection character pointer.
 * Only the first instantiation is used.
 */
void inputRedirectHandler(char *input, struct Command *c) {
    if (c->iredir == NULL) {
        char *s;

        s = (char *)calloc(sizeof(input) + 1, sizeof(char));
        strcpy(s, input);

        c->iredir = s;
    }
}

/*
 * Sets the Command struct's output redirection character pointer.
 * Only the first instantiation is used.
 */
void outputRedirectHandler(char *input, struct Command *c) {
    if (c->oredir == NULL) {
        char *s;

        s = (char *)calloc(sizeof(input) + 1, sizeof(char));
        strcpy(s, input);

        c->oredir = s;
    }
}

/*
 * Sets the Command struct's background process integer to
 * 0 or 1 which represents false and true, respectively.
 */
void setBackgroundProcess(int bool, struct Command *c) {
    c->bg = bool;
}