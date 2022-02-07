#ifndef SIGNAL_HANDLERS_H_
#define SIGNAL_HANDLERS_H_

void handleSIGTSTP(int);
void handleSIGUSR2(int);
void initializeSIGTSTP(void);
void initializeSignalHandlers(void);
void initializeSIGUSR2(void);
int terminatedBySIGINT(int);

#endif