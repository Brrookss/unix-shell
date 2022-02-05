#ifndef COMMANDS_BUILT_IN_H_
#define COMMANDS_BUILT_IN_H_

int isBuiltInCommand(struct Command *);
int iscd(char *);
int isexit(char *);
int isstatus(char *);

void cd_(struct Command *);
void executeBuiltInCommand(struct Command *, struct ShellProcess *);
void exit_(struct ShellProcess *);
void status(struct ShellProcess *);

#endif