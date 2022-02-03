#ifndef COMMAND_PROMPT_H_
#define COMMAND_PROMPT_H_

struct Command {
    char *name;
    char *iredir;
    char *oredir;
    int bg;
    char *args[];
};

char *getInput(void);

int isBackgroundProcess(char *);
int isComment(char *);
int isInputRedirect(char *);
int isOutputRedirect(char *);
int isPromptSymbol(char *);

struct Command *initializeCommandStruct(void);
struct Command *parseInput(char *);

#endif