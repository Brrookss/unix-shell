#ifndef MEMORY_H_
#define MEMORY_H_

#include "command_prompt.h"
#include "shell_process.h"

void deallocateInput(char *);
void deallocateCommand(struct Command *);
void deallocateShellProcess(struct ShellProcess *);

#endif