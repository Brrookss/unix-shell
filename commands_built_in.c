#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "command_prompt.h"
#include "command_handlers.h"
#include "commands_built_in.h"

/*
 *
 */
void cd(struct Command *c) {
    c->args[1] ? chdir(c->args[1]) : chdir(getenv("HOME"));
}

/*
 *
 */
void executeBuiltInCommand(struct Command *c) {
    if (iscd(c->name)) {
        cd(c);
    }
}

/*
 * Checks if Command's name matches a built-in command
 */
int isBuiltInCommand(struct Command *c) {
    return iscd(c->name) || isexit(c->name) || isstatus(c->name);
}

/*
 * Checks if string matches "cd"
 */
int iscd(char *s) {
    return strcmp(s, "cd") == 0;
}

/*
 * Checks if string matches "exit"
 */
int isexit(char *s) {
    return strcmp(s, "exit") == 0;
}

/*
 * Checks if string matches "status"
 */
int isstatus(char *s) {
    return strcmp(s, "status") == 0;
}