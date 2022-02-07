#ifndef COMMANDS_EXTERNAL_H_
#define COMMANDS_EXTERNAL_H_

int executeExternalCommandBackground(struct Command *, struct ShellProcess *);
int executeExternalCommandForeground(struct Command *, struct ShellProcess *);
int runInBackgroundAttempt(struct Command *);

#endif