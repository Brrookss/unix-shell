#ifndef SMALLSH_H_
#define SMALLSH_H_

void alternateForegroundOnly(void);
int backgroundProcessesAllowed(void);
int getForegroundPID(void);
void setForegroundPID(int);
void setStatusMessage(char*);

#endif