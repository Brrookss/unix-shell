#ifndef __SIGNAL_HANDLERS_H__
#define __SIGNAL_HANDLERS_H__

void handleSIGTSTP(int);
void handleSIGUSR2(int);
void initializeSIGTSTP(void);
void initializeSignalHandlers(void);
void initializeSIGUSR2(void);
int terminatedBySIGINT(int);

#endif /* __SIGNAL_HANDLERS_H__ */