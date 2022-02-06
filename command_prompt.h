#ifndef COMMAND_PROMPT_H_
#define COMMAND_PROMPT_H_

#define MAX_CL_CHARS 2048
#define MAX_ARGS 512

struct Command {
    char *name;
    char **args;
    char *input_redir;
    char *output_redir;
    int foreground;
};

char *getInput(void);

int isBackgroundProcess(char *);
int isComment(char *);
int isInputRedirect(char *);
int isOutputRedirect(char *);
int redirectStdin(struct Command *);
int redirectStdout(struct Command *);
int stdinRedirectAttempt(struct Command *);
int stdoutRedirectAttempt(struct Command *);
int successfulRedirects(int, int);

struct Command *initializeCommand(void);
struct Command *parseInput(char *);

void resetStdin(int);
void resetStdout(int);

#endif