#ifndef COMMAND_HANDLERS_H_
#define COMMAND_HANDLERS_H_

char *expandVar(char *, char *);
char *hasVarExpansion(char *);

int isCommand(struct Command *);

void argumentHandler(char *, struct Command *);
void commandHandler(char *, struct Command *);
void inputRedirectHandler(char *, struct Command *);
void outputRedirectHandler(char *, struct Command *);
void setBackgroundProcess(int, struct Command *);

#endif