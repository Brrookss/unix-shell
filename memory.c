#include <stdlib.h>
#include "command_prompt.h"

/*
 * Deallocates user input character array
 */
void deallocateInput(char *s) {
    free(s);
    s = NULL;
}

/*
 * Deallocates Command structure
 */
void deallocateCommandStruct(struct Command *c) {
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