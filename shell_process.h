#ifndef __SHELL_PROCESS_H__
#define __SHELL_PROCESS_H__

#define STATUS_MESSAGE_CHARS 100

struct BackgroundProcess {
    struct BackgroundProcess *next;
    int pid;
};

struct ShellProcess {
    struct BackgroundProcess* head;
    char* prev_status_message;
    int exiting;
};

void addBackgroundProcess(int, struct ShellProcess*);
void checkBackgroundProcesses(struct ShellProcess*);
int deleteBackgroundProcess(int, struct ShellProcess*);
void displayPrevStatusMessage(struct ShellProcess*);
int foundBackgroundProcess(int);
int hasDifferentStatusMessage(char*, struct ShellProcess*);
struct ShellProcess* initializeShellProcess(void);
int isRunning(struct ShellProcess*);
void setExitFailureMessage(struct ShellProcess*);
int setPrevStatusMessage(int, struct ShellProcess*);
void updatePrevStatusMessage(char*, struct ShellProcess*);

#endif /* __SHELL_PROCESS_H__ */