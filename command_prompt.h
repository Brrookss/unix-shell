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
struct Command *parseInput(char *);
struct Command *initializeCommandStruct(void);
int isBackgroundProcess(char *);
int isComment(char *);
int isInputRedirect(char *);
int isOutputRedirect(char *);
int isPromptSymbol(char *);

#endif