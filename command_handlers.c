#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "command_prompt.h"
#include "command_handlers.h"

#define MAX_PID_DIGITS 8
#define VARIABLE_EXPANSION "$$"

/*
 * Adds to the Command structure's argument character pointer array
 * with the index being the first NULL element
 */
void argumentHandler(char *input, struct Command *c) {
    char *s;
    int i;

    s = (char *)calloc(strlen(input) + 1, sizeof(char));
    strcpy(s, input);

    i = 0;
    while (c->args[i])
        i++;
    
    c->args[i] = s;
}

/*
 * Sets the Command structure's name character pointer.
 * Only the first instantiation is used.
 */
void commandHandler(char *input, struct Command *c) {
    if (c->name == NULL) {
        char *s;

        s = (char *)calloc(sizeof(input) + 1, sizeof(char));
        strcpy(s, input);

        c->name = s;
    }
}

/*
 * Creates a new string in which the pid replaces the variable expansion
 * characters.
 * Only the first instance of the variable expansion characters is
 * replaced so multiple calls are needed if more than one pair exists.
 */
char *expandVar(char *input, char *substring) {
    int pid_i, i, j, k;
    char *output, pid_s[MAX_PID_DIGITS] = {'\0'};

    pid_i = getpid();
    sprintf(pid_s, "%d", pid_i);

    output = (char *)calloc(strlen(input) + strlen(pid_s), sizeof(char));

    // Copy original string into new string until variable expansion characters
    i = j = 0;
    while (sizeof(char) * i + input < substring) {
        output[i++] = input[j++];
    }

    // Copy ASCII-converted pid into new string
    k = 0;
    while (pid_s[k] != '\0') {
        output[i++] = pid_s[k++];
    }

    // Copy rest of original string after variable expansion characters
    j += strlen(VARIABLE_EXPANSION);
    while (input[j] != '\0') {
        output[i++] = input[j++];
    }
    return output;
}

/*
 * Checks if string has at least one pair of variable expansion
 * characters. A character pointer to the first instance is
 * returned, otherwise the pointer will be NULL.
 */
char *hasVarExpansion(char *input) {
    return strstr(input, VARIABLE_EXPANSION);
}

/*
 * Sets the Command structure's input redirection character pointer.
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
 * Checks if there is a command name stored in the Command structure
 */
int isCommand(struct Command *c) {
    return c->name != NULL;
}

/*
 * Sets the Command structure's output redirection character pointer.
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
 * Sets the Command structure's foreground process integer to
 * 0 or 1 which represents true and false, respectively
 */
void setBackgroundProcess(int bool, struct Command *c) {
    c->foreground = bool;
}