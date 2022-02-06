#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
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
 * Creates an empty Command structure
 */
struct Command *initializeCommand(void) {
    struct Command *c;

    c = (struct Command *)malloc(sizeof(struct Command));

    c->args = calloc(MAX_ARGS + 1, sizeof(char *));
    c->name = NULL;
    c->input_redir = NULL;
    c->output_redir = NULL;
    c->foreground = 0;

    return c;
}

/*
 * Checks if string is or starts with the comment symbol
 */
int isComment(char *s) {
    return strcmp(s, "#") == 0 || strncmp(s, "#", 1) == 0;
}

/*
 * Checks if string matches the input redirection symbol
 */
int isInputRedirect(char *s) {
    return strcmp(s, "<") == 0;
}

/*
 * Checks if string matches the output redirection symbol
 */
int isOutputRedirect(char *s) {
    return strcmp(s, ">") == 0;
}

/*
 * Checks if string matches the shell prompt symbol
 */
int isPrompt(char *s) {
    return strcmp(s, ":") == 0;
}

/*
 * Checks if string matches the background process invocation symbol
 */
int isBackgroundProcess(char *s) {
    return strcmp(s, "&") == 0;
}

/*
 * Interprets user input and stores data in a Command structure to be utilized
 */
struct Command *parseInput(char *input) {
    char *tok, *next_tok, *substr, *delims = " \n";
    struct Command *c;

    c = initializeCommand();

    tok = strtok(input, delims);

    if (tok && isPrompt(tok)) {
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
            stdinRedirectHandler(next_tok, c);
        }
        else if (isOutputRedirect(tok)) {
            next_tok = strtok(NULL, delims);
            stdoutRedirectHandler(next_tok, c);
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
 * Redirects standard input to the file path stored in the Command structure
 */
int redirectStdin(struct Command *c) {
	int input_fd, saved_stdin;
    
    input_fd = open(c->input_redir, O_RDONLY);

	if (input_fd == -1) { 
		printf("Cannot open %s for input\n", c->input_redir);
        fflush(stdout);
		return -1; 
	}

	saved_stdin = dup(STDIN_FILENO);
    dup2(input_fd, STDIN_FILENO);

    return saved_stdin;
}

/*
 * Redirects standard output to the file path stored in the Command structure
 */
int redirectStdout(struct Command *c) {
	int output_fd, saved_stdout;
    
    output_fd = open(c->output_redir, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (output_fd == -1) { 
		printf("Cannot open %s for output\n", c->output_redir);
        fflush(stdout);
		return -1;
	}
  
    saved_stdout = dup(STDOUT_FILENO);
	dup2(output_fd, STDOUT_FILENO);

    return saved_stdout;
}

/*
 * Restores standard input
 */
void resetStdin(int stdin) {
    dup2(stdin, STDIN_FILENO);
}

/*
 * Restores standard output
 */
void resetStdout(int stdout) {
    dup2(stdout, STDOUT_FILENO);
}

/*
 * Checks if standard input and standard output were redirected succesfully
 */
int successfulRedirects(int stdin, int stdout) {
    return stdin != -1 && stdout != -1;
}

/*
 * Checks if there's an attempt to redirect standard input
 */
int stdinRedirectAttempt(struct Command *c) {
    return c->input_redir != NULL;
}

/*
 * Checks if there's an attempt to redirect standard output
 */
int stdoutRedirectAttempt(struct Command *c) {
    return c->output_redir != NULL;
}