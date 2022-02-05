#ifndef SHELL_PROCESS_H_
#define SHELL_PROCESS_H_

struct BackgroundProcess {
    struct BackgroundProcess *next;
    int pid;
};

struct ShellProcess {
    struct BackgroundProcess *head;
    char *prev_status_message;
    int exiting;
};


int isRunning(struct ShellProcess *);

struct ShellProcess *initializeShellProcess(void);

void addBackgroundProcess(int, struct ShellProcess *);
void checkBackgroundProcesses(struct ShellProcess *);
void deleteBackgroundProcess(int, struct ShellProcess *);
void setPrevStatusMessage(int, struct ShellProcess *);

#endif