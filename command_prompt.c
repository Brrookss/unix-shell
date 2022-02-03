#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_prompt.h"
#include "command_handlers.h"

#define CL_BUFFER_SIZE 2048
#define ARGS_BUFFER_SIZE 512

/*
 * Gets user input to be parsed
 */
char *getInput(void) {
    char *buffer;

    buffer = (char *)calloc(CL_BUFFER_SIZE + 1, sizeof(char));
    fgets(buffer, CL_BUFFER_SIZE + 1, stdin);
    return buffer;
}

/*
 * Interprets user input and stores data in a struct to be utilized
 */
struct Command *parseInput(char *input) {
    char *tok, *next_tok, *substr, *delims = " \n\t";
    struct Command *c;

    c = initializeCommandStruct();

    tok = strtok(input, delims);

    if (tok && isPromptSymbol(tok)) {
        tok = strtok(NULL, delims);
    } else {
        tok = NULL;  // Ensures lines without prompt symbol won't be parsed further
    }

    // Interprets non-comment token as a command
    if (tok && !isComment(tok)) {
        commandHandler(tok, c);
        tok = strtok(NULL, delims);
    } else {
        tok = NULL;  // Ensures comment lines won't be parsed further
    }

    // Inteprets arguments, input/output redirections, and background processing invocations
    while (tok) {
        if (isInputRedirect(tok)) {
            next_tok = strtok(NULL, delims);  // Gets following related argument
            inputRedirectHandler(next_tok, c);
        }

        else if (isOutputRedirect(tok)) {
            next_tok = strtok(NULL, delims);
            outputRedirectHandler(next_tok, c);
        }

        else if (isBackgroundProcess(tok)) {
            setBackgroundProcess(1, c);
        }

        else {
            while ((substr = hasVarExpansion(tok)) != NULL) {
                tok = expandVar(tok, substr);
            }
            argumentHandler(tok, c);
        }
        tok = strtok(NULL, delims);
    }

    // Deallocates input array
    free(input);
    input = NULL;

    return c;
}

/*
 * Checks if string is a pound symbol or starts with one
 */
int isComment(char *s) {
    return strcmp(s, "#") == 0 || strncmp(s, "#", 1) == 0;
}

/*
 * Checks if string is a less than symbol
 */
int isInputRedirect(char *s) {
    return strcmp(s, "<") == 0;
}

/*
 * Checks if string is a greater than symbol
 */
int isOutputRedirect(char *s) {
    return strcmp(s, ">") == 0;
}

/*
 * Checks if string is a colon
 */
int isPromptSymbol(char *s) {
    return strcmp(s, ":") == 0;
}

/*
 * Checks if string is an ampersand
 */
int isBackgroundProcess(char *s) {
    return strcmp(s, "&") == 0;
}

/*
 * Creates an empty Command structure
 */
struct Command *initializeCommandStruct(void) {
    struct Command *c;

    c = (struct Command *)malloc(sizeof(struct Command));
    c->name = NULL;
    memset(c->args, '\0', ARGS_BUFFER_SIZE * sizeof(char *));
    c->iredir = NULL;
    c->oredir = NULL;
    c->bg = 0;

    return c;
}