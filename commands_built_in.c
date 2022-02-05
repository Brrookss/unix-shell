#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "command_prompt.h"
#include "memory.h"
#include "shell_process.h"
#include "command_handlers.h"
#include "commands_built_in.h"

/*
 * Changes current directory.
 * Path is given as an argument and can be absolute or relative.
 * If path isn't provided, the default is the path specified
 * in the HOME environment variable.
 */
void cd_(struct Command *c) {
    c->args[1] ? chdir(c->args[1]) : chdir(getenv("HOME"));
}

/*
 * Executes intended built-in command
 */
void executeBuiltInCommand(struct Command *c, struct ShellProcess *sh) {
    if (iscd(c->name)) {
        cd_(c);
    } else if (isstatus(c->name)) {
        status(sh);
    } else {
        exit_(sh);
    }
}

/*
 * Initiates shell exit
 */
void exit_(struct ShellProcess *sh) {
    sh->exiting = 0;
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

/*
 * Displays the signal of the most recent foreground process
 */
void status(struct ShellProcess *sh) {
    printf("%s\n", sh->prev_status_message);
    fflush(stdout);
}