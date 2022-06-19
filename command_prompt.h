#ifndef __COMMAND_PROMPT_H__
#define __COMMAND_PROMPT_H__

#define MAX_CL_CHARS 2048
#define MAX_ARGS 512

struct Command {
    char* name;
    char** args;
    char* input_redir;
    char* output_redir;
    int foreground;
};

char* getInput(void);
struct Command* initializeCommand(void);
int isBackgroundProcess(char*);
int isComment(char*);
int isInputRedirect(char*);
int isOutputRedirect(char*);
struct Command* parseInput(char*);
int redirectStdin(struct Command*);
int redirectStdout(struct Command*);
void resetStdin(int);
void resetStdout(int);
int stdinRedirectAttempt(struct Command*);
int stdoutRedirectAttempt(struct Command*);
int successfulRedirects(int, int);

#endif /* __COMMAND_PROMPT_H__ */