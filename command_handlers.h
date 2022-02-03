#ifndef COMMAND_HANDLERS_H_
#define COMMAND_HANDLERS_H_

void argumentHandler(char *, struct Command *);
void commandHandler(char *, struct Command *);
void inputRedirectHandler(char *, struct Command *);
void outputRedirectHandler(char *, struct Command *);
void setBackgroundProcess(int, struct Command *);

#endif