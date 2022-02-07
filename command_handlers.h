#ifndef COMMAND_HANDLERS_H_
#define COMMAND_HANDLERS_H_

#define MAX_PID_DIGITS 8
#define VARIABLE_EXPANSION "$$"

void argumentHandler(char *, struct Command *);
void commandHandler(char *, struct Command *);
char *expandVar(char *, char *);
char *hasVarExpansion(char *);
int isCommand(struct Command *);
void setBackgroundProcess(int, struct Command *);
void stdinRedirectHandler(char *, struct Command *);
void stdoutRedirectHandler(char *, struct Command *);

#endif