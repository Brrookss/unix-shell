#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_prompt.h"
#include "command_handlers.h"

/*
 * Gets user input to be parsed
 */
char *getInput(void) {
    char *buffer;
    size_t len = MAX_CL_CHARS + 1;

    buffer = (char *)calloc(MAX_CL_CHARS + 1, sizeof(char));
    getline(&buffer, &len, stdin);
    return buffer;
}

/*
 * Interprets user input and stores data in a Command structure to be utilized
 */
struct Command *parseInput(char *input) {
    char *tok, *next_tok, *substr, *delims = " \n";
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
        argumentHandler(tok, c);  // Includes command as first argument
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

    c->args = calloc(MAX_ARGS + 1, sizeof(char *));
    c->name = NULL;
    c->iredir = NULL;
    c->oredir = NULL;
    c->foreground = 0;

    return c;
}