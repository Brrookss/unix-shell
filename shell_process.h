#ifndef SHELL_PROCESS_H_
#define SHELL_PROCESS_H_

struct BackgroundProcess {
    struct BackgroundProcess *next;
    int pid;
};

struct ShellProcess {
    struct BackgroundProcess *head;
    int prev_term_signal;
    int exiting;
};

struct ShellProcess *initializeShellProcessStruct(void);
void addBackgroundProcess(int, struct ShellProcess *);
void checkBackgroundProcesses(struct ShellProcess *);
void deleteBackgroundProcess(int, struct ShellProcess *);
void setPrevTermSignal(int, struct ShellProcess *);

#endif