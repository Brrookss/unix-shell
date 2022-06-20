#ifndef __COMMANDS_EXTERNAL_H__
#define __COMMANDS_EXTERNAL_H__

int executeExternalCommandBackground(struct Command*, struct ShellProcess*);
int executeExternalCommandForeground(struct Command*, struct ShellProcess*);
int runInBackgroundAttempt(struct Command*);

#endif /* __COMMANDS_EXTERNAL_H__ */