#ifndef __COMMANDS_BUILT_IN_H__
#define __COMMANDS_BUILT_IN_H__

int isBuiltInCommand(struct Command*);
int iscd(char*);
int isexit(char*);
int isstatus(char*);

void cd_(struct Command*);
void executeBuiltInCommand(struct Command*, struct ShellProcess*);
void exit_(struct ShellProcess*);
void status(struct ShellProcess*);

#endif /* __COMMANDS_BUILT_IN_H__ */