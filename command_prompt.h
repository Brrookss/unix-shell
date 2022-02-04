#ifndef COMMAND_PROMPT_H_
#define COMMAND_PROMPT_H_

#define MAX_CL_CHARS 2048
#define MAX_ARGS 512

struct Command {
    char *name;
    char **args;
    char *iredir;
    char *oredir;
    int fg;
};

char *getInput(void);

int isBackgroundProcess(char *);
int isComment(char *);
int isInputRedirect(char *);
int isOutputRedirect(char *);
int isPromptSymbol(char *);

struct Command *initializeCommandStruct(void);
struct Command *parseInput(char *);

void deallocateCommandStruct(struct Command *c);

#endif