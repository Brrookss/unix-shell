#ifndef __SHELL_H__
#define __SHELL_H__

void alternateForegroundOnly(void);
int backgroundProcessesAllowed(void);
int getForegroundPID(void);
void setForegroundPID(int);
void setStatusMessage(char*);

#endif /* __SHELL_H__ */